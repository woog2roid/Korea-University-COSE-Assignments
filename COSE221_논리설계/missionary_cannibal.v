module missionary_cannibal(missionary_curr, cannibal_curr, direction, missionary_next, cannibal_next);
		
	input [1:0] missionary_curr, cannibal_curr;
	input direction;
	output [1:0] missionary_next, cannibal_next;
	
	wire dirN, mc1N, mc0N, cc1N, cc0N;
	not(dirN, direction);
	not(mc1N, missionary_curr[1]);
	not(mc0N, missionary_curr[0]);
	not(cc1N, cannibal_curr[1]);
	not(cc0N, cannibal_curr[0]);

	wire a1, a11;
	or(a1, missionary_curr[1], missionary_curr[0], cc1N);
	or(a11, a1, cannibal_curr[0]);
	
	wire b, b1, b11, b12, b2,b3;
	or(b, mc1N, mc0N);
	or(b1, b, cannibal_curr[1]);
	or(b11, b1, cc0N);
	or(b12, b1, cannibal_curr[0]);
	or(b2, b, cc0N);
	or(b3,b,cc1N);
	
	wire c1, c11;
	or(c1, missionary_curr[1], cannibal_curr[1],cc0N);
	or(c11, c1, missionary_curr[0]);
	
	wire d1, d11;
	or(d1, mc1N, cc1N, cannibal_curr[0]);
	or(d11, d1, missionary_curr[0]);

	wire x1, x0, y1, y0, z1, z0, w1, w0;
	and(x1, direction, a1, b11, d11);
	and(x0, dirN, c11, a11);
	and(y1, direction, a1, d11);
	and(y0, dirN, c1, a11);
	and(z1, direction, a1, b2, b3);
	and(z0, dirN, b12);
	and(w1, direction, a11, d1);
	and(w0, dirN, c1, b11);
	
	or(missionary_next[1],x1,x0);
	or(missionary_next[0],y1,y0);
	or(cannibal_next[1],z1,z0);
	or(cannibal_next[0],w1,w0);
	
endmodule