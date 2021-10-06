#ifndef UI_VEC_H
# define UI_VEC_H

# define VEC2_SIZE 2
# define VEC4_SIZE 4

typedef struct s_vec2
{
	union {
		struct {
			float x;
			float y;
		};
		float v[VEC2_SIZE];
	};
}					t_vec2;

typedef struct s_vec2i
{
	union {
		struct {
			int x;
			int y;
		};
		int v[VEC2_SIZE];
	};
}					t_vec2i;

typedef struct s_vec4
{
	union {
		struct {
			float x;
			float y;
			float w;
			float h;
		};
		float v[VEC4_SIZE];
	};
}					t_vec4;

typedef struct s_vec4i
{
	union {
		struct {
			int x;
			int y;
			int w;
			int h;
		};
		int v[VEC4_SIZE];
	};
}					t_vec4i;

// Vec2
t_vec2				vec2(float x, float y);
t_vec2i				vec2i(int x, int y);
void				vec2i_swap(t_vec2i *v1, t_vec2i *v2);
void				vec2_swap(t_vec2 *v1, t_vec2 *v2);
t_vec2i				vec2i_add(t_vec2i v1, t_vec2i v2);

// Vec4
t_vec4i				vec4i(int x, int y, int w, int h);
t_vec4				vec4(float x, float y, float w, float h);
void				vec4i_swap(t_vec4i *v1, t_vec4i *v2);
void				vec4_swap(t_vec4 *v1, t_vec4 *v2);

#endif
