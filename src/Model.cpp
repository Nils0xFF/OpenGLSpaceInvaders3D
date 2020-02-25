//
//  Model.cpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Model.h"
#include "phongshader.h"
#include <list>
#include <fstream>

Model::Model() : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
}
Model::Model(const char* ModelFile, float fitScale, bool Center) : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
	this->fitScale = fitScale;
    bool ret = load(ModelFile, fitScale, Center);
    if(!ret)
        throw std::exception();
}
Model::~Model()
{
    // TODO: Add your code (Exercise 3)
    deleteNodes(&RootNode);
    delete [] this->pMeshes;
    for(unsigned int i = 0; i < MaterialCount; i++){
        Texture::ReleaseShared(this->pMaterials[i].DiffTex);
		Texture::ReleaseShared(this->pMaterials[i].NormalTex);
    }
    delete [] this->pMaterials;
}

void Model::deleteNodes(Node* pNode)
{
    if(!pNode)
        return;
    for(unsigned int i=0; i<pNode->ChildCount; ++i)
        deleteNodes(&(pNode->Children[i]));
    if(pNode->ChildCount>0)
        delete [] pNode->Children;
    if(pNode->MeshCount>0)
        delete [] pNode->Meshes;
}

bool Model::load(const char* ModelFile, float fitScale, bool Center)
{
    const aiScene* pScene = aiImportFile( ModelFile,aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords );
    
    if(pScene==NULL || pScene->mNumMeshes<=0)
        return false;
    
    Filepath = ModelFile;
    Path = Filepath;
    size_t pos = Filepath.rfind('/');
    if(pos == std::string::npos)
        pos = Filepath.rfind('\\');
    if(pos !=std::string::npos)
        Path.resize(pos+1);
    
    loadMeshes(pScene, fitScale, Center);
    loadMaterials(pScene);
    loadNodes(pScene);
    
	aiReleaseImport(pScene);
    return true;
}

void Model::loadMeshes(const aiScene* pScene, float fitScale, bool Center)
{
    
    float scaleFactor = 1;
	Vector offset = Vector::zero();

    // TODO: Add your code (Exercise 3)
    if(pScene->HasMeshes()){
        
        this->pMeshes = new Mesh[pScene->mNumMeshes];
        this->MeshCount = pScene->mNumMeshes;
        this->calcBoundingBox(pScene, this->InitialBoundingBox);


        if(fitScale > 0){
            float largest = std::max(std::max(this->InitialBoundingBox.size().X, this->InitialBoundingBox.size().Y), this->InitialBoundingBox.size().Z);
            scaleFactor = fitScale / largest;
            this->InitialBoundingBox.Max = this->InitialBoundingBox.Max * scaleFactor;
            this->InitialBoundingBox.Min = this->InitialBoundingBox.Min * scaleFactor;
        }

		if (Center) {
			offset = -InitialBoundingBox.center();
			InitialBoundingBox.Min += offset;
			InitialBoundingBox.Max += offset;
		}
        
		this->updateBoundingBox();

        for(unsigned int i = 0; i< pScene->mNumMeshes; i++){
            this->pMeshes[i].VB.begin();
            this->pMeshes[i].IB.begin();
            
            for(unsigned int j = 0; j < pScene->mMeshes[i]->mNumVertices; j++){
                if(pScene->mMeshes[i]->HasNormals()){
                    this->pMeshes[i].VB.addNormal(pScene->mMeshes[i]->mNormals[j].x, pScene->mMeshes[i]->mNormals[j].y, pScene->mMeshes[i]->mNormals[j].z );
                }
                
                if(pScene->mMeshes[i]->HasTextureCoords(0)){
                    this->pMeshes[i].VB.addTexcoord0(pScene->mMeshes[i]->mTextureCoords[0][j].x, -pScene->mMeshes[i]->mTextureCoords[0][j].y, 0);
                }
                
                if(pScene->mMeshes[i]->HasTangentsAndBitangents()){
                    this->pMeshes[i].VB.addTexcoord1(pScene->mMeshes[i]->mTangents[j].x, pScene->mMeshes[i]->mTangents[j].y, pScene->mMeshes[i]->mTangents[j].z);
                    this->pMeshes[i].VB.addTexcoord2(pScene->mMeshes[i]->mBitangents[j].x, pScene->mMeshes[i]->mBitangents[j].y, pScene->mMeshes[i]->mBitangents[j].z);
                }
                
                
                this->pMeshes[i].VB.addVertex(pScene->mMeshes[i]->mVertices[j].x * scaleFactor + offset.X, pScene->mMeshes[i]->mVertices[j].y * scaleFactor + offset.Y, pScene->mMeshes[i]->mVertices[j].z * scaleFactor + offset.Z);
            }
            
            if(pScene->mMeshes[i]->HasFaces()){
                for(unsigned int j = 0; j < pScene->mMeshes[i]->mNumFaces; j++){
                    for(unsigned int k = 0; k < pScene->mMeshes[i]->mFaces->mNumIndices; k++){
                        this->pMeshes[i].IB.addIndex(pScene->mMeshes[i]->mFaces[j].mIndices[k]);
                    }
                }
            }
            
            this->pMeshes[i].MaterialIdx = pScene->mMeshes[i]->mMaterialIndex;
            this->pMeshes[i].VB.end();
            this->pMeshes[i].IB.end();
        }
        
        
        
    }
}
void Model::loadMaterials(const aiScene* pScene)
{
    // TODO: Add your code (Exercise 3)
    if(pScene->HasMaterials()){
        this->pMaterials = new Material[pScene->mNumMaterials];
        this->MaterialCount = pScene->mNumMaterials;
        for(unsigned int i = 0; i< pScene->mNumMaterials; i++){
            this->pMaterials[i] = Material();
            
            aiColor4D diffuse, specular, ambient;
            aiGetMaterialColor(pScene->mMaterials[i], AI_MATKEY_COLOR_DIFFUSE, &diffuse);
            aiGetMaterialColor(pScene->mMaterials[i], AI_MATKEY_COLOR_SPECULAR, &specular);
            aiGetMaterialColor(pScene->mMaterials[i], AI_MATKEY_COLOR_AMBIENT, &ambient);
            
            this->pMaterials[i].DiffColor = Color(diffuse.r, diffuse.g, diffuse.b);
            this->pMaterials[i].SpecColor = Color(specular.r, specular.g, specular.b);
            this->pMaterials[i].AmbColor = Color(ambient.r, ambient.g, ambient.b);
            
            aiGetMaterialFloat(pScene->mMaterials[i], AI_MATKEY_SHININESS, &(this->pMaterials[i].SpecExp));
            
            aiString diffTex;
            aiGetMaterialString(pScene->mMaterials[i], AI_MATKEY_TEXTURE_DIFFUSE(0), &diffTex);
            this->pMaterials[i].DiffTex = Texture::LoadShared((this->Path + diffTex.C_Str()).c_str());
            
            std::string normalTex = (this->Path + diffTex.C_Str());
            normalTex.insert(normalTex.rfind("."), "_n");

            std::ifstream ifile(normalTex);
            if (ifile) {
                this->pMaterials[i].NormalTex = Texture::LoadShared(normalTex.c_str());
            }else {
                this->pMaterials[i].NormalTex = Texture::defaultNormalTex();
            }
            
        }
    }
}
void Model::calcBoundingBox(const aiScene* pScene, AABB& Box)
{
    // TODO: Add your code (Exercise 3)
    float largestX = -INFINITY, largestY = -INFINITY, largestZ = -INFINITY;
    float smallestX = INFINITY, smallestY = INFINITY, smallestZ = INFINITY;
    
    if(pScene->HasMeshes()){
        for(unsigned int i = 0; i< pScene->mNumMeshes; i++){
            for(unsigned int j = 0; j < pScene->mMeshes[i]->mNumVertices; j++){
                if(pScene->mMeshes[i]->mVertices[j].x > largestX){
                    largestX = pScene->mMeshes[i]->mVertices[j].x;
                }
                if(pScene->mMeshes[i]->mVertices[j].y > largestY){
                    largestY = pScene->mMeshes[i]->mVertices[j].y;
                }
                if(pScene->mMeshes[i]->mVertices[j].z > largestZ){
                    largestZ = pScene->mMeshes[i]->mVertices[j].z;
                }
                if(pScene->mMeshes[i]->mVertices[j].x < smallestX){
                    smallestX = pScene->mMeshes[i]->mVertices[j].x;
                }
                if(pScene->mMeshes[i]->mVertices[j].y < smallestY){
                    smallestY = pScene->mMeshes[i]->mVertices[j].y;
                }
                if(pScene->mMeshes[i]->mVertices[j].z < smallestZ){
                    smallestZ = pScene->mMeshes[i]->mVertices[j].z;
                }
            }
        }
        
    }
    
    Box.Max = Vector(largestX, largestY, largestZ);
    Box.Min = Vector(smallestX, smallestY, smallestZ);
	this->InitialBoundingBox = Box;
}

void Model::loadNodes(const aiScene* pScene)
{
    deleteNodes(&RootNode);
    copyNodesRecursive(pScene->mRootNode, &RootNode);
}

void Model::copyNodesRecursive(const aiNode* paiNode, Node* pNode)
{
    pNode->Name = paiNode->mName.C_Str();
    pNode->Trans = convert(paiNode->mTransformation);
    
    if(paiNode->mNumMeshes > 0)
    {
        pNode->MeshCount = paiNode->mNumMeshes;
        pNode->Meshes = new int[pNode->MeshCount];
        for(unsigned int i=0; i<pNode->MeshCount; ++i)
            pNode->Meshes[i] = (int)paiNode->mMeshes[i];
    }
    
    if(paiNode->mNumChildren <=0)
        return;
    
    pNode->ChildCount = paiNode->mNumChildren;
    pNode->Children = new Node[pNode->ChildCount];
    for(unsigned int i=0; i<paiNode->mNumChildren; ++i)
    {
        copyNodesRecursive(paiNode->mChildren[i], &(pNode->Children[i]));
        pNode->Children[i].Parent = pNode;
    }
}

void Model::applyMaterial( unsigned int index)
{
    if(index>=MaterialCount)
        return;
    
    PhongShader* pPhong = dynamic_cast<PhongShader*>(shader());
    if(!pPhong) {
        //        std::cout << "Model::applyMaterial(): WARNING Invalid shader-type. Please apply PhongShader for rendering models.\n";
        return;
    }
    
    Material* pMat = &pMaterials[index];
    pPhong->ambientColor(pMat->AmbColor);
    pPhong->diffuseColor(pMat->DiffColor);
    pPhong->specularExp(pMat->SpecExp);
    pPhong->specularColor(pMat->SpecColor);
    pPhong->diffuseTexture(pMat->DiffTex);
    pPhong->normalTexture(pMat->NormalTex);
}

void Model::draw(const BaseCamera& Cam)
{
    if(!pShader) {
        std::cout << "BaseModel::draw() no shader found" << std::endl;
        return;
    }
    pShader->modelTransform(transform());
    
    std::list<Node*> DrawNodes;
    DrawNodes.push_back(&RootNode);
    
    while(!DrawNodes.empty())
    {
        Node* pNode = DrawNodes.front();
        Matrix GlobalTransform;
        
        if(pNode->Parent != NULL)
            pNode->GlobalTrans = pNode->Parent->GlobalTrans * pNode->Trans;
        else
            pNode->GlobalTrans = transform() * pNode->Trans;
        
        pShader->modelTransform(pNode->GlobalTrans);
        
        for(unsigned int i = 0; i<pNode->MeshCount; ++i )
        {
            Mesh& mesh = pMeshes[pNode->Meshes[i]];
            mesh.VB.activate();
            mesh.IB.activate();
            applyMaterial(mesh.MaterialIdx);
            pShader->activate(Cam);
            glDrawElements(GL_TRIANGLES, mesh.IB.indexCount(), mesh.IB.indexFormat(), 0);
            mesh.IB.deactivate();
            mesh.VB.deactivate();
        }
        for(unsigned int i = 0; i<pNode->ChildCount; ++i )
            DrawNodes.push_back(&(pNode->Children[i]));
        
        DrawNodes.pop_front();
    }
}

Matrix Model::convert(const aiMatrix4x4& m)
{
    return Matrix(m.a1, m.a2, m.a3, m.a4,
                  m.b1, m.b2, m.b3, m.b4,
                  m.c1, m.c2, m.c3, m.c4,
                  m.d1, m.d2, m.d3, m.d4);
}



