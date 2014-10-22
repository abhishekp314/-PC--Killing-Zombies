#include "Model.h"

Model::Model(LPDIRECT3DDEVICE9 dev)
{
	d3ddev=dev;
}

Model::~Model()
{
	DeleteModel();
}

void Model::LoadModel(char *filename,char *folder)
{
	foldername=folder;
	model=(MODEL*)malloc(sizeof(MODEL));
	LPD3DXBUFFER matBuffer;
	HRESULT result;
	
	//Load Mesh
	result=D3DXLoadMeshFromX(
		filename,
		D3DXMESH_SYSTEMMEM,
		d3ddev,
		NULL,
		&matBuffer,
		NULL,
		&model->material_count,
		&model->mesh);
	if(result!=D3D_OK)
	{
		MessageBox(NULL,"Error loading Model","Error",MB_OK);
		exit(0);
		return;
	}

	//EXTRACT MATERIALS PROPERTIES AND TEXTURE NAMES FROM MATERIAL BUFFER
	D3DXMATERIAL* d3dxMaterials=(D3DXMATERIAL*)matBuffer->GetBufferPointer();
	model->materials=new D3DMATERIAL9[model->material_count];
	model->textures=new LPDIRECT3DTEXTURE9[model->material_count];

	//CREATE THE MATERIAL AND TEXTURE
	char text[1024];

	for(DWORD i=0;i<model->material_count;i++)
	{
		//GRAB MATERIAL
		model->materials[i]=d3dxMaterials[i].MatD3D;

		//set ambient color ofor material
		model->materials[i].Ambient=model->materials[i].Diffuse;

		model->textures[i]=NULL;

		if(d3dxMaterials[i].pTextureFilename!=NULL && lstrlen(d3dxMaterials[i].pTextureFilename)>0)
		{
			//LOAD TEXTURE FILE SPECIFIED IN .X FILE
			wsprintf(text,"C:\\Users\\Tinku\\Documents\\Visual Studio 2012\\Projects\\Speed_v3_DX9\\Speed_v3_DX9\\Content\\Models\\%s\\%s",foldername,d3dxMaterials[i].pTextureFilename);
			result=D3DXCreateTextureFromFile(d3ddev,text,&model->textures[i]);
			//MessageBox(NULL,text,"Check FIlenames",MB_OK);
		}
	}
	
	//DONE USING MATERIAL BUFFER
	matBuffer->Release();
	return;
}


void Model::DeleteModel()
{
	//remove materials from memory
	if(model->materials!=NULL)
		delete[] model->materials;

	//remove texture
	if(model->textures!=NULL)
	{
		for(DWORD i=0;i<model->material_count;i++)
		{
			if(model->textures[i]!=NULL)
				model->textures[i]->Release();
		}
		delete[] model->textures;
	}

	//remove mesh from memory
	if(model->mesh!=NULL)
		model->mesh->Release();

	//remove model struct from memory
	if(model!=NULL)
		free(model);
}


void Model::DrawModel()
{
	//DRAW EACH OF THE MESH SUBSETS
	for(DWORD i=0;i<model->material_count;i++)
	{
		//SET THE MATERIAL AND TEXTURE OF THIS SUBSET
		d3ddev->SetMaterial(&model->materials[i]);
		d3ddev->SetTexture(0,model->textures[i]);

		//draw the mesh subset
		model->mesh->DrawSubset(i);
	}
}

void Model::SetModelAttributes()
{
	pData=0;
	model->mesh->LockVertexBuffer(D3DLOCK_READONLY,(VOID**)&pData);
	numVertices=model->mesh->GetNumVertices();
	numBytesVertices=model->mesh->GetNumBytesPerVertex();
	model->mesh->UnlockVertexBuffer();
}