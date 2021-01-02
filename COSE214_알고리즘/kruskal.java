/*
INPUT
The first line is given the number of vertices V (1 ≤ V ≤ 10,000) and the number edges E (1 ≤ E ≤ 100,000).
The following lines of E are given three integers, A, B and C, indicating information for each edge.
This means that the vertex A and the vertex B are connected by a edge with a weight of C. C may be negative, and the absolute value does not exceed 1,000,000.
The vertices of the graph are numbered from 1 to V, and there is a path between any two vertices.
Only data with a minimum spanning tree weight greater than or equal to -2,147,483,648 and less than or equal to 2,147,483,647 are given as inputs.

OUTPUT
The edges of the minimum spanning tree is printed on lines.
The weight of the minimum spanning tree is printed on the following line.
 */

import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;

public class Main {
    static int [] unionFind;
    public static void main(String[] args) {
        //INPUT
        Scanner sc = new Scanner(System.in);
        int V = sc.nextInt(), E = sc.nextInt();
        ArrayList<Edge> list = new ArrayList<>();
        for(int i=0; i<E; i++)  list.add(new Edge(sc.nextInt(), sc.nextInt(), sc.nextInt()));

        //sort for the Kruskal's algorithm
        Collections.sort(list);
        //initialize the union find array.
        unionFind = new int[V+1];
        for(int i=1; i<=V; i++) unionFind[i] = i;

        //compute
        long res = 0;
        for(int i=0; i<E; i++) {
            int from = list.get(i).from;
            int to = list.get(i).to;
            int weight = list.get(i).weight;

            if(findRoot(from)!=findRoot(to)) {//To not make a cycle
                unionFind[findRoot(to)] = unionFind[from];
                res += list.get(i).weight;
                System.out.println(from + " --> "+ to +", weight:"+ weight);
            }
        }

        //OUTPUT
        System.out.println("The sum of weight of all edges: " + res);
    }

    //find the root of the node
    static int findRoot(int num) {
        if(num != unionFind[num])   //not yet reach to root node
            return unionFind[num] = findRoot(unionFind[num]);
        else    //arrive root node
            return num;
    }
}

class Edge implements Comparable<Edge>{
    int from, to, weight;

    Edge(int from, int to, int weight) {
        this.from = from;
        this.to = to;
        this.weight = weight;
    }

    //To sort the arrayList
    @Override
    public int compareTo(Edge o) {
        return this.weight-o.weight;
    }
}