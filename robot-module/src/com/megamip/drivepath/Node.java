package com.megamip.drivepath;

import java.util.ArrayList;

public class Node {
	
	private int id;
	private int weight;
	private ArrayList<Integer> neighbors;
	

	// constructors 
	public Node() {
		super();
	}
	public Node(int id, int weight) {
		super();
		this.id = id;
		this.weight = weight;
	}
	
	
	// getters & setters 
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public int getWeight() {
		return weight;
	}
	public void setWeight(int weight) {
		this.weight = weight;
	}
	
	
	public ArrayList<Integer> getNeighbors() {
		return neighbors;
	}
	public void setNeighbors(ArrayList<Integer> neighbors) {
		this.neighbors = neighbors;
	}
	

	
}
