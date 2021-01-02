/*
INPUT
The first line is given the number of vertices V and the number of edges E (1≤V≤20,000, 1≤E≤300,000).
Assume that all vertices are numbered from 1 to V. The second line is given the starting vertex number K (1≤K≤V).
Three integers (u, v, and w) representing each edge are given in order from the third line to the Eth line.
This means that there is a edge whose weight is w from u to v.
U and v are different and w is a natural number of not more than 10.
Note that there may be multiple edges between two different vertices.

OUTPUT
From the first line to the V line, the i line prints the shortest path to the i vertex.
The starting point itself can be outputted as zero, and print "INF"  if the path does not exist.
 */

import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Scanner;

public class Main{
    static int INF = 100000000;
    static ArrayList<Pair>[] list;
    static int d[];

    public static void main(String[] args){
        //INPUT
        Scanner sc = new Scanner(System.in);
        int V = sc.nextInt(), E = sc.nextInt();
        int k = sc.nextInt();//the "from" index

        list = new ArrayList[V+1];
        d = new int[V+1];
        for(int i=1; i<=V; i++) {
            //initialize
            d[i] = INF;
            list[i] = new ArrayList<Pair>();
        }
        for(int i=0; i<E; i++) {//from = Array List's index, to = Pair.index, d=Pair.distance
            list[sc.nextInt()].add(new Pair(sc.nextInt(), sc.nextInt()));
        }

        //compute
        dijkstra(k);

        //OUTPUT
        System.out.println("from index "+k);
        for(int i=1; i<=V; i++) {
            if(d[i] == INF) System.out.println("to index "+i+": INF");
            else System.out.println("to index "+i+ ": "+d[i]);
        }
    }

    public static void dijkstra(int start) {
        //Using Heap to optimize the time complexity
        PriorityQueue<Pair> pq = new PriorityQueue<Pair>();

        d[start] = 0;
        pq.offer(new Pair(start, d[start]));

        while(!pq.isEmpty()) {//heap is more efficient than array
            Pair p = pq.peek();
            int current = p.index, distance = p.distance;
            pq.poll();
            if(d[current] < distance)   continue;

            for(int i=0; i<list[current].size(); i++) {
                int next = list[current].get(i).index;
                int nextDistance = distance + list[current].get(i).distance;

                if(nextDistance < d[next]) {//if there is shorter route
                    d[next] = nextDistance;
                    pq.offer(new Pair(next, nextDistance));
                }
            }
        }
    }

}

class Pair implements Comparable<Pair>{
    int index;
    int distance;

    Pair(int index, int distance) {
        this.index = index;
        this.distance = distance;
    }

    //To implement priority queue
    @Override
    public int compareTo(Pair o) {
        return this.distance - o.distance;
    }
}