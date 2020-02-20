package com.wowwee.drivepath;

import java.util.ArrayList;

public class Node {
	
	private int id;
	private int weight;
	private ArrayList<Integer> neighbors;
	

	// constructors 
	public Node() {
		super();
	}
	@Override
	public String toString() {
		return "Node [id=" + id + ", weight=" + weight + ", neighbors="
				+ neighbors + "]";
	}
	public Node(int id, int weight) {
		super();
		this.id = id;
		this.weight = weight;
		neighbors = new ArrayList<Integer>();
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
	
@Override
public boolean equals(Object o) {
	Node n1  = this;
	Node n2 = (Node)o;
	return n1.getId() == n2.getId();
}


	
}
