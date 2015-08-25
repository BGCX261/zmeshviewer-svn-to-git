#pragma warning(once:4819)

#include "../Core_Coin/Control_Node.h"
#include <Inventor/Win/SoWin.h>
#include <iostream>

//Add Face Mesh

void help( char* name );

int main(int argc , char ** argv)
{
 	if( argc < 2 )
 	{
 		help( argv[0] );
 		exit(-1);
 	}
	
	ZCoinLib::Control_Node control( argv[0] );

 	for( int i = 0; i < argc - 1; i ++ )
 	{
 		control.AddMesh( argv[i+1] );
 	}
	
	control.Show();
	return 0;
}
void help( char* name )
{
	std::cout<< "Mesh Viewer\n";
	std::cout<< "Usage:\n";
	//std::cout<< name << "face.m skull.m\n";
	std::cout<< name << " mesh1.m mesh2.m ...\n";
	std::cout<< "\n";
	std::cout<< "Functions:\n";
	std::cout<< "   Read in several mesh files                 [Done]\n";
	std::cout<< "   [1~9] hide/show mesh                       [Done]\n";
	std::cout<< "   Select vertex                              [Done]\n";
	std::cout<< "	Show mesh with useful info                       \n";
	std::cout<< "        -- Vertex/face/edge number            [Done]\n";
	std::cout<< "        -- Component number                   [Done]\n";
	std::cout<< "        -- Genus                              [Done]\n";
	std::cout<< "        -- Traits                             [Done]\n";
	std::cout<< "   Show vertex of a given id                  [Done]\n";
	std::cout<< "   Show a command gui                     [Not Done]\n";
	std::cout<< "   ...\n";
}
