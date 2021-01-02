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
import java.util.PriorityQueue;
import java.util.Scanner;

public class Main {
    static final int INF = 2147483647;
    static final int MAX = 10000;
    static ArrayList<Pair> [] list;

    public static void main(String[] args) {
        //INPUT
        Scanner sc = new Scanner(System.in);
        int V = sc.nextInt(), E = sc.nextInt();
        list = new ArrayList[MAX+1];
        for(int i=1; i<=V; i++) list[i] = new ArrayList<Pair>();
        for (int i = 0; i < E; i++) {
            int from = sc.nextInt(), to = sc.nextInt(), weight = sc.nextInt();
            list[from].add(new Pair(to,weight));
            list[to].add(new Pair(from, weight));
        }

        //compute
        int res = prim(1);

        //OUTPUT
        System.out.println(res);
    }

    static int prim(int start){
        PriorityQueue<Pair> pq = new PriorityQueue<>();
        //To optimize the time complexity
        //The node of miniest weight will be pop

        boolean [] visit = new boolean[MAX+1];
        for(int i=0; i<MAX+1; i++)  visit[i] = false;
        visit[start] = true;
        //To check the visited node

        for (int i = 0; i < list[start].size(); i++)
        {
            // push to the Priority Queue
            int index = list[start].get(i).index;
            int weight = list[start].get(i).weight;
            pq.add(new Pair(index, weight));
        }

        int res = 0;
        int from = start;
        //To print the edges, store the index of precedence node
        while(!pq.isEmpty()) {
            int index = pq.peek().index;
            int weight = pq.peek().weight;
            pq.poll();

            // already visited --> continue
            if (visit[index]) continue;
            else {
                visit[index] = true;
                System.out.println(from + " --> "+ index +", weight:"+ weight);
                res += weight;

                //Push the next Node
                for(int i=0; i<list[index].size(); i++){
                    int tmpIndex = list[index].get(i).index;
                    int tmpWeight = list[index].get(i).weight;
                    pq.add(new Pair(tmpIndex, tmpWeight));
                }
                from = index;
            }
        }

        return res;
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

class Pair implements Comparable<Pair>{
    int index;
    int weight;

    Pair(int index, int weight) {
        this.index = index;
        this.weight = weight;
    }

    //To implement priority queue
    @Override
    public int compareTo(Pair o) {
        return this.weight - o.weight;
    }
}