#pragma once

class Scene;

class Framework
{
public:
	Framework();
	~Framework();
	void MainLoop();
	
private:
	Scene* root_;
};