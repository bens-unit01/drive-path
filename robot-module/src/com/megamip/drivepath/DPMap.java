package com.megamip.drivepath;

import java.util.LinkedList;

public class DPMap {
	
	
	private LinkedList<Node> nodes;
	private int current = 0;

	// constructors 
	public DPMap() {
		super();
		nodes = new LinkedList<Node>();
	}


	public DPMap(LinkedList<Node> nodes) {
		super();
		this.nodes = nodes;
	}
	
	
	public int getCurrent() {
		return current;
	}


	public void setCurrent(int current) {
		this.current = current;
	}


	// getters & setters 
	public LinkedList<Node> getNodes() {
		return nodes;
	}

	public void setNodes(LinkedList<Node> nodes) {
		this.nodes = nodes;
	}
	
   public void add(Node newNode) {
	nodes.add(newNode); 
   }
}
