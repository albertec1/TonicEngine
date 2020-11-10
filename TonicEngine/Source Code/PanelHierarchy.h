#ifndef __PANELHIERARCHY_H__
#define __PANELHIERARCHY_H__

#include "Application.h"
#include "PanelManager.h"
#include "GameObject.h"


class PanelHierarchy : public PanelManager
{
public:
	PanelHierarchy();
	~PanelHierarchy();

	bool Start();
	bool Draw();
	void DrawMenuHovering();
	void DrawMenuNotHovering();

	bool openMenuHovering = false;
	bool openMenuNotHovering = false;

	uint clickedGO = -1;
	bool notActiveColor = true;
	void ManageNodesOnHierarchy(GameObject* GO);

private:
};

#endif
