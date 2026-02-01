#pragma once
#include <string>
#include <vector>
#include <SDL3/SDL.h>

class Node {
public:
    std::string name;           // Name of the node
	Node* parent = nullptr;     // Pointer to the parent node
	std::vector<Node*> children;// List of child nodes

	Node(const std::string& name); // Constructor
	virtual ~Node(); // Virtual destructor

	void AddChild(Node* child);     // Add a child node
	void RemoveChild(Node* child);  // Remove a child node

	virtual void EnterTree();						// Called when the node is added to the scene tree
	virtual void ExitTree();						// Called when the node is removed from the scene tree
	virtual void Process(double delta);				// Called every frame to update the node
	virtual void Render(SDL_GPURenderPass* pass);	// Called to render the node
};