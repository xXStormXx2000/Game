#pragma once
#include "DynamicArray.h"
#include <unordered_map>
#include <map>

/*
 * A node that's going to be used needs to implement:
 *
 * A variable named: dis
 *   - Represents the distance from the starting node to this node.
 *
 * A variable named: preNode
 *   - Points to the previous node in the optimal path.
 *
 * A function named: distance
 *   - Takes in a pointer to a node and returns an approximate distance
 *     (heuristic) between itself and the node it takes in.
 *
 * A function named: getAdjacentNodes
 *   - Returns a DynamicArray containing pairs of adjacent node pointers
 *     and the distances between the current node and the adjacent node.
 *
 */

 // Template function implementing the A* algorithm
template<typename NodePtr, typename Dis>
DynamicArray<NodePtr> aStar(NodePtr sNodePtr, NodePtr eNodePtr, Dis& dis) {

	// If the start node is the same as the end node, the shortest path is the node itself
	if (sNodePtr == eNodePtr) {
		dis = 0;
		return { sNodePtr };
	}

	// Priority queue (map) for nodes, with distance as key
	std::map<Dis, DynamicArray<NodePtr>> queue;
	// Lookup table to store the shortest distance to each node
	std::unordered_map<NodePtr, Dis> disLookUp;

	// Start from the source node
	NodePtr node = sNodePtr;
	node->dis = 0;  // Set the distance of the start node to 0

	//declaring the nextNodePtr
	NodePtr nextNodePtr;

	// Calculate the heuristic (estimated distance) from the current node to the goal
	Dis temp = node->distance(eNodePtr);
	// Add the start node to the queue with the heuristic as key
	queue[temp].pushBack(node);
	// Store the start node's heuristic value in the lookup table
	disLookUp[node] = temp;

	Dis outdatedDis;  // Variable to track outdated distance values

	// DynamicArray to hold adjacent nodes and their distances
	DynamicArray<std::pair<NodePtr, Dis>> adjacentNodePtrs;

	// Main loop of A* algorithm - continues until we reach the goal node
	while (node != eNodePtr) {
		// Get adjacent nodes to the current node
		adjacentNodePtrs = node->getAdjacentNodes();

		// Iterate through all adjacent nodes
		for (std::pair<NodePtr, Dis> ad : adjacentNodePtrs) {
			// Skip processing the node if it's the same as the current node
			if (ad.first == node) continue;

			// Get the adjacent node pointer
			nextNodePtr = ad.first;
			// Update the distance of the adjacent node
			nextNodePtr->dis = ad.second + node->dis;
			// Recalculate the heuristic from the adjacent node to the goal
			temp = nextNodePtr->distance(eNodePtr);

			// If the adjacent node is not yet processed or the new distance is better
			if (disLookUp.find(nextNodePtr) == disLookUp.end() || disLookUp[nextNodePtr] > nextNodePtr->dis + temp) {
				// Update the previous node of the adjacent node to the current node
				nextNodePtr->preNode = node;
				// Add the adjacent node to the queue with the updated distance
				queue[nextNodePtr->dis + temp].pushBack(nextNodePtr);

				// If the adjacent node was already in the queue, remove the outdated entry
				if (disLookUp.find(nextNodePtr) != disLookUp.end()) {
					outdatedDis = disLookUp[nextNodePtr];
					queue[outdatedDis].erase(nextNodePtr);
					// If there are no nodes with the outdated distance, remove the entry
					if (queue[outdatedDis].size() <= 0) queue.erase(outdatedDis);
				}

				// Update the lookup table with the new shortest distance
				disLookUp[nextNodePtr] = nextNodePtr->dis + temp;
			}
		}

		// Remove the current node from the queue since we've processed it
		temp = disLookUp[node];
		queue[temp].erase(node);
		// If there are no nodes with the same distance, remove the queue entry
		if (queue[temp].size() <= 0) queue.erase(temp);

		// If the queue is empty, return an empty result (no path found)
		if (queue.empty()) {
			dis = -1;  // Indicating no valid path
			return {};
		}

		// Select the next node from the queue (the one with the lowest cost)
		node = queue.begin()->second.back();
	}

	// Build the path from the end node back to the start node
	DynamicArray<NodePtr> list;
	dis = node->dis;  // The final distance to the end node
	// Backtrack through the preNode pointers to reconstruct the path
	while (node != sNodePtr) {
		list.pushFront(node);
		node = node->preNode;
	}
	list.pushFront(node);  // Add the start node to the front of the list

	// Return the reconstructed path
	return list;
}