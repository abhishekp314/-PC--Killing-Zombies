#include<d3dx9.h>

#ifndef _Model_h
#define _Model_h
class Model
{

public :LPDIRECT3DDEVICE9 d3ddev;
public:
	Model(LPDIRECT3DDEVICE9 dev);
	~Model();
	
	typedef struct MODEL
	{
	LPD3DXMESH mesh;
	D3DMATERIAL9* materials;
	LPDIRECT3DTEXTURE9* textures;
	DWORD material_count;
	
	};

	MODEL *model;
	char *foldername;
	BYTE* pData;
	DWORD numVertices;
	DWORD numBytesVertices;

	 void LoadModel(char *filename,char *folder);
	 void DeleteModel();
	 void DrawModel();
	 void SetModelAttributes();

};

#endif