#include "Node.h"
#include <algorithm>


// Constructor
Node::Node(const std::string& name) : name(name) {}

// Destructor
Node::~Node() {
    for (Node* child : children) delete child;
    children.clear();
}

// Add a child node
void Node::AddChild(Node* child) {
	if (child->parent) child->parent->RemoveChild(child); // If the child already has a parent then remove from previous parent

	child->parent = this;       // Set this node as the parent

	children.push_back(child);  // Add to children list
    
	child->EnterTree();         // Call EnterTree on the child
}

void Node::RemoveChild(Node* child) {
	auto it = std::remove(children.begin(), children.end(), child); // Find the child in the children list

    if (it != children.end()) {
		// If found, remove it
        children.erase(it);
        child->parent = nullptr;
        child->ExitTree();
    }
}

void Node::EnterTree() {}
void Node::ExitTree() {}

void Node::Process(double delta) {
    for (Node* child : children) child->Process(delta);
}

void Node::Render(SDL_GPURenderPass* pass) {
    for (Node* child : children) child->Render(pass);
}