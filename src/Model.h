//
//  Model.hpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Model_hpp
#define Model_hpp

#include <stdio.h>
#include "basemodel.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "texture.h"
#include "aabb.h"
#include <string>

class Model : public BaseModel
{
public:
    Model();
    Model(const char* ModelFile, bool FitSize=true, bool Center=true);
	Model(const Model& other) :BaseModel(other) {};
	Model* clone() { 
		Model* newModel = new Model(Filepath.c_str(), true);
		newModel->shader(pShader->clone(), true);
		newModel->transform(this->transform());
	return newModel;
	}
    virtual ~Model();

    bool load(const char* ModelFile, bool FitSize=true, bool Center = true);
    virtual void draw(const BaseCamera& Cam);
	void updateBoundingBox() { this->BoundingBox = this->InitialBoundingBox.transform(Transform); }
	const AABB& initialBoundingBox() const { return InitialBoundingBox; }
    const AABB& boundingBox() const { return BoundingBox; }

protected: // protected types
    struct Mesh
    {
        Mesh()  {}
        VertexBuffer VB;
        IndexBuffer IB;
        int MaterialIdx = -1;
    };
    struct Material
    {
        Material() : DiffTex(NULL), NormalTex(NULL), DiffColor(1,1,1),SpecColor(0.3f,0.3f,0.3f), AmbColor(0,0,0), SpecExp(10) {}
        Color DiffColor;
        Color SpecColor;
        Color AmbColor;
        float SpecExp;
        const Texture* DiffTex;
        const Texture* NormalTex;
    };
    struct Node
    {
        Node() : Parent(NULL), Children(NULL), ChildCount(0), MeshCount(0), Meshes(NULL) {}
        Matrix Trans;
        Matrix GlobalTrans;
        int* Meshes;
        unsigned int MeshCount;
        Node* Parent;
        Node* Children;
        unsigned int ChildCount;
        std::string Name;
    };
    
protected: // protected methods
    void loadMeshes(const aiScene* pScene, bool FitSize, bool Center);
    void loadMaterials(const aiScene* pScene);
    void calcBoundingBox( const aiScene* pScene, AABB& Box);

    void loadNodes(const aiScene* pScene);
    void copyNodesRecursive(const aiNode* paiNode, Node* pNode);
    Matrix convert(const aiMatrix4x4& m);
    void applyMaterial( unsigned int index);
    void deleteNodes(Node* pNode);

protected: // protected member variables
    Mesh* pMeshes = NULL;
    unsigned int MeshCount = 0;
    Material* pMaterials = NULL;
    unsigned int MaterialCount = 0;
    AABB InitialBoundingBox;
	AABB BoundingBox;
    
    std::string Filepath; // stores pathname and filename
    std::string Path; // stores path without filename
    Node RootNode;
    
};

#endif /* Model_hpp */
