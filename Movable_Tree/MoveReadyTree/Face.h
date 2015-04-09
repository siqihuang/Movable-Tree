#ifndef FACE_H
#define FACE_H 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#define EDGE_EDGE_TEST(V0,U0,U1)                      \
  Bx=U0[i0]-U1[i0];                                   \
  By=U0[i1]-U1[i1];                                   \
  Cx=V0[i0]-U0[i0];                                   \
  Cy=V0[i1]-U0[i1];                                   \
  f=Ay*Bx-Ax*By;                                      \
  d=By*Cx-Bx*Cy;                                      \
  if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
  {                                                   \
    e=Ax*Cy-Ay*Cx;                                    \
    if(f>0)                                           \
    {                                                 \
      if(e>=0 && e<=f) return 1;                      \
    }                                                 \
    else                                              \
    {                                                 \
      if(e<=0 && e>=f) return 1;                      \
    }                                                 \
  }                      

#define EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2) \
{                                              \
  double Ax,Ay,Bx,By,Cx,Cy,e,d,f;               \
  Ax=V1[i0]-V0[i0];                            \
  Ay=V1[i1]-V0[i1];                            \
  /* test edge U0,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U0,U1);                    \
  /* test edge U1,U2 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U1,U2);                    \
  /* test edge U2,U1 against V0,V1 */          \
  EDGE_EDGE_TEST(V0,U2,U0);                    \
}

#define POINT_IN_TRI(V0,U0,U1,U2)           \
{                                           \
  double a,b,c,d0,d1,d2;                     \
  /* is T1 completly inside T2? */          \
  /* check if V0 is inside tri(U0,U1,U2) */ \
  a=U1[i1]-U0[i1];                          \
  b=-(U1[i0]-U0[i0]);                       \
  c=-a*U0[i0]-b*U0[i1];                     \
  d0=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U2[i1]-U1[i1];                          \
  b=-(U2[i0]-U1[i0]);                       \
  c=-a*U1[i0]-b*U1[i1];                     \
  d1=a*V0[i0]+b*V0[i1]+c;                   \
                                            \
  a=U0[i1]-U2[i1];                          \
  b=-(U0[i0]-U2[i0]);                       \
  c=-a*U2[i0]-b*U2[i1];                     \
  d2=a*V0[i0]+b*V0[i1]+c;                   \
  if(d0*d1>0.0)                             \
  {                                         \
    if(d0*d2>0.0) return 1;                 \
  }                                         \
}

#define   sVpsV_2( Vr, s1,  V1,s2, V2);\
	{\
  Vr[0] = s1*V1[0] + s2*V2[0];\
  Vr[1] = s1*V1[1] + s2*V2[1];\
}\

// 2D intersection of segment and triangle.
#define seg_collide3( q, r)\
{\
	p1[0]=SF*P1[0];\
	p1[1]=SF*P1[1];\
	p2[0]=SF*P2[0];\
	p2[1]=SF*P2[1];\
	det1 = p1[0]*q[1]-q[0]*p1[1];\
	gama1 = (p1[0]*r[1]-r[0]*p1[1])*det1;\
	alpha1 = (r[0]*q[1] - q[0]*r[1])*det1;\
	alpha1_legal = (alpha1>=0) && (alpha1<=(det1*det1)  && (det1!=0));\
	det2 = p2[0]*q[1] - q[0]*p2[1];\
	alpha2 = (r[0]*q[1] - q[0]*r[1]) *det2;\
	gama2 = (p2[0]*r[1] - r[0]*p2[1]) * det2;\
	alpha2_legal = (alpha2>=0) && (alpha2<=(det2*det2) && (det2 !=0));\
	det3=det2-det1;\
	gama3=((p2[0]-p1[0])*(r[1]-p1[1]) - (r[0]-p1[0])*(p2[1]-p1[1]))*det3;\
	if (alpha1_legal)\
	{\
		if (alpha2_legal)\
		{\
			if ( ((gama1<=0) && (gama1>=-(det1*det1))) || ((gama2<=0) && (gama2>=-(det2*det2))) || (gama1*gama2<0)) return true;\
		}\
		else\
		{\
			if ( ((gama1<=0) && (gama1>=-(det1*det1))) || ((gama3<=0) && (gama3>=-(det3*det3))) || (gama1*gama3<0)) return true;\
			}\
	}\
	else\
	if (alpha2_legal)\
	{\
		if ( ((gama2<=0) && (gama2>=-(det2*det2))) || ((gama3<=0) && (gama3>=-(det3*det3))) || (gama2*gama3<0)) return true;\
		}\
	return false;\
}

class Face 
{
public:
	int index; 
	std::vector<unsigned int>vertex_indexs; //each vertex index
	std::vector<glm::vec3>vertex_coords; //each vertex coordinates 
	std::vector<unsigned int>uv_indexs; //each vertex's uv index
	std::vector<glm::vec2>uv_coords; //each vertex's uv coordinates 

	glm::vec3 min_pt;
	glm::vec3 max_pt;

	//4.1 union-find
	int rank;	
	Face* parent; 

	//4.3 Compute F-domain graph
	//it the shape of face is quad, then separate a quad into 2 triangles for later collision detection.
	//vertex coordinates
	std::vector<glm::vec3>C1, C2;


    Face()
	{
		rank = 0;
		parent = this;
	}

	Face(const std::vector<unsigned int>&_vertex_indexs, const std::vector<glm::vec3>&_vertex_coords,
		const std::vector<unsigned int>&_uv_indexs, const std::vector<glm::vec2>&_uv_coords) 
	{
		rank = 0;
		parent = this;
		vertex_coords = _vertex_coords;
		vertex_indexs = _vertex_indexs;
		uv_indexs = _uv_indexs; 
		uv_coords = _uv_coords; 
		//compute max_pt, min_pt
		max_pt = min_pt = vertex_coords[0];
		for(int i = 1; i < vertex_coords.size(); ++i)
		{
			glm::vec3 tmp = vertex_coords[i];
			min_pt.x = std::min(min_pt.x, tmp.x); 
			min_pt.y = std::min(min_pt.y, tmp.y); 
			min_pt.z = std::min(min_pt.z, tmp.z); 

			max_pt.x = std::max(max_pt.x, tmp.x); 
			max_pt.y = std::max(max_pt.y, tmp.y); 
			max_pt.z = std::max(max_pt.z, tmp.z); 
		}
		//is quad. So separate a quad into 2 triangles
		//for collision detection
		if(vertex_coords.size() == 4)
		{
			C1.resize(3);
			C2.resize(3);
			//Triangle a: 1
			C1[0] = vertex_coords[0];
			C1[1] = vertex_coords[1];
			C1[2] = vertex_coords[2];
			
			//Triangle a: 2 
			C2[0] = vertex_coords[0];
			C2[1] = vertex_coords[2];
			C2[2] = vertex_coords[3];
		}
	}
	
	bool CheckShareVertex(Face *b)
	{
		for(int i = 0; i < vertex_indexs.size(); ++i)
		{
			for(int j = 0; j < b->vertex_indexs.size(); ++j)
			{
				if(vertex_indexs[i] == b->vertex_indexs[j]) //same vertex index
				{
					return true;
				}
			}
		}
		return false;
	}

	static bool coplanar_tri_tri(glm::vec3 N,glm::vec3 V0,glm::vec3 V1,glm::vec3 V2, glm::vec3 U0,glm::vec3 U1,glm::vec3 U2)
	{
	   glm::vec3 A;
	   short i0,i1;
	   /* first project onto an axis-aligned plane, that maximizes the area */
	   /* of the triangles, compute indices: i0,i1. */
	   A[0]=fabs(N[0]);
	   A[1]=fabs(N[1]);
	   A[2]=fabs(N[2]);
	   if(A[0]>A[1])
	   {
		  if(A[0]>A[2])  
		  {
			  i0=1;      /* A[0] is greatest */
			  i1=2;
		  }
		  else
		  {
			  i0=0;      /* A[2] is greatest */
			  i1=1;
		  }
	   }
	   else   /* A[0]<=A[1] */
	   {
		  if(A[2]>A[1])
		  {
			  i0=0;      /* A[2] is greatest */
			  i1=1;                                           
		  }
		  else
		  {
			  i0=0;      /* A[1] is greatest */
			  i1=2;
		  }
		}               
					
		/* test all edges of triangle 1 against the edges of triangle 2 */
		EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2);
		EDGE_AGAINST_TRI_EDGES(V1,V2,U0,U1,U2);
		EDGE_AGAINST_TRI_EDGES(V2,V0,U0,U1,U2);
					
		/* finally, test if tri1 is totally contained in tri2 or vice versa */
		POINT_IN_TRI(V0,U0,U1,U2);
		POINT_IN_TRI(U0,V0,V1,V2);
		return false;
	}
	
	//arguments: Triangle 1 vertex coordinates; Triangle 2 vertex coordinates; 
	static bool TriangleIntersectionTest(const std::vector<glm::vec3>&C, const std::vector<glm::vec3>&D) 
	{
		glm::vec3 P1 = C[1] - C[0]; 
		glm::vec3 P2 = C[2] - C[0]; 

		glm::vec3 Q1 = D[1] - D[0]; 
		glm::vec3 Q2 = D[2] - D[0]; 

		glm::vec3 t, p1,p2,r,r4;
		double beta1, beta2, beta3;
		double gama1, gama2, gama3;
		double det1, det2, det3;
		double dp0, dp1, dp2;
		double dq1,dq2,dq3,dr, dr3;
		double alpha1, alpha2;
		bool alpha1_legal, alpha2_legal;
		double  SF;
		bool beta1_legal, beta2_legal;
				
		r = D[0] - C[0];
		// determinant computation	
		dp0 = P1[1]*P2[2]-P2[1]*P1[2];
		dp1 = P1[0]*P2[2]-P2[0]*P1[2];
		dp2 = P1[0]*P2[1]-P2[0]*P1[1];
		dq1 = Q1[0]*dp0 - Q1[1]*dp1 + Q1[2]*dp2;
		dq2 = Q2[0]*dp0 - Q2[1]*dp1 + Q2[2]*dp2;
		dr  = -r[0]*dp0  + r[1]*dp1  - r[2]*dp2;

		beta1 = dr*dq2;  // beta1, beta2 are scaled so that beta_i=beta_i*dq1*dq2
		beta2 = dr*dq1;
		beta1_legal = (beta2>=0) && (beta2 <=dq1*dq1) && (dq1 != 0);
		beta2_legal = (beta1>=0) && (beta1 <=dq2*dq2) && (dq2 != 0);
			
		dq3=dq2-dq1;
		dr3=+dr-dq1;   // actually this is -dr3
		
		if ((dq1 == 0) && (dq2 == 0))
		{
			// triangles are on parallel planes
			if (dr!=0) return false;  
			else  // triangles are on the same plane
			{						
				glm::vec3 C2, C3, D2, D3, N1;
				// We use the coplanar test of Moller which takes the 6 vertices and 2 normals  as input.
				C2 = C[0] + P1;
				C3 = C[0] + P2;
				D2 = D[0] + Q1;
				D3 = D[0] + Q2;
				N1 = glm::cross(P1,P2);
				return coplanar_tri_tri(N1,C[0], C2,C3,D[0],D2,D3);
			}
		}

		else if (!beta2_legal && !beta1_legal) return false;// fast reject-all vertices are on
														// the same side of the triangle plane

		else if (beta2_legal && beta1_legal)    //beta1, beta2
		{
			SF = dq1*dq2;
			sVpsV_2(t,beta2,Q2, (-beta1),Q1);
		}
		
		else if (beta1_legal && !beta2_legal)   //beta1, beta3
		{
			SF = dq1*dq3;
			beta1 =beta1-beta2;   // all betas are multiplied by a positive SF
			beta3 =dr3*dq1;
			sVpsV_2(t,(SF-beta3-beta1),Q1,beta3,Q2);
		}
		
		else if (beta2_legal && !beta1_legal) //beta2, beta3
		{
			SF = dq2*dq3;
			beta2 =beta1-beta2;   // all betas are multiplied by a positive SF
			beta3 =dr3*dq2;
			sVpsV_2(t,(SF-beta3),Q1,(beta3-beta2),Q2);
			Q1=Q2;
			beta1=beta2;
		}
		sVpsV_2(r4,SF,r,beta1,Q1);
		seg_collide3(t,r4);  // calculates the 2D intersection
		return false;
	}	

	float GetUVdistance(const Face* b)
	{
		int n = uv_coords.size();
		if(n != b->uv_coords.size())
		{
			//printf("jason: %d %d\n", uv_coords.size(), b->uv_coords.size());
			return FLT_MAX;
		}
		bool *vis = new bool[n];
		for(int i = 0; i < n; ++i)
		{
			vis[i] = false;
		}
		for(int i = 0; i < n; ++i)
		{
			bool found = false;
			for(int j = 0;  j < n; ++j)
			{
				if(vis[j])continue;
				float tmp_dis = fabs(uv_coords[i].x - b->uv_coords[j].x) + fabs(uv_coords[i].y - b->uv_coords[j].y); 
				//greedy
				if(tmp_dis < 0.01f) 
				{
					found = true;
					vis[j] = true;
					break;
				}
			}
			if(!found)
			{
				delete[ ] vis;
				return FLT_MAX;
			}
		}
		delete[ ] vis;
		return 0;
	}
};
#endif