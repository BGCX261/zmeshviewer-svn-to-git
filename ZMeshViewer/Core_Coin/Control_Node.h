#pragma warning(once:4819)

#ifndef __CONTROL_NODE_H__
#define __CONTROL_NODE_H__

#include "Common_Coin.h"
#include "Coin_Node.h"

#include <Inventor/Win/SoWin.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/actions/SoSearchAction.h>

#include <string>
#include <vector>

BEGIN_DEFINE_COIN_NAMESPACE

class Control_Node
{
public:
	typedef std::string		StrMeshName;

public:
	Control_Node( char* win_name );
	~Control_Node();

	void InitialCoin();
	void AddMesh( char* mesh_name );
	
	void Show();

private:
	char*					m_win_name;
	HWND					m_window;

	SoWinExaminerViewer*	m_viewer;
	SoSeparator*			m_root;

	Coin_Node*				m_coin_node;

	std::vector<StrMeshName> m_mesh_name_vec;
};

END_DEFINE_COIN_NAMESPACE

#endif //__CONTROL_NODE_H__