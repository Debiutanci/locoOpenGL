#version 330

uniform mat4 P; //projekcja
uniform mat4 V; //view
uniform mat4 M; //model

in vec4 vertex;
in vec4 color;
in vec4 normal;
in vec2 texCoord;

out vec4 ic;
out vec4 l1;
out vec4 l2;
out vec4 l3;
out vec4 n;
out vec4 v;
out vec2 iTexCoord;
out vec2 iTexCoord1;

void main(void) {
    vec4 lp1 = vec4(0, 20, -6, 1); // Pozycja pierwszego œwiat³a, przestrzeñ œwiata
    vec4 lp2 = vec4(-10, 0, 5, 1); // Pozycja drugiego œwiat³a, przestrzeñ œwiata
    vec4 lp3 = vec4(5, -5, 15, 1); // Pozycja trzeciego œwiat³a, przestrzeñ œwiata

    l1 = normalize(V * lp1 - V * M * vertex); // Wektor do pierwszego œwiat³a w przestrzeni oka
    l2 = normalize(V * lp2 - V * M * vertex); // Wektor do drugiego œwiat³a w przestrzeni oka
    l3 = normalize(V * lp3 - V * M * vertex); // Wektor do trzeciego œwiat³a w przestrzeni oka
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); // Wektor do obserwatora w przestrzeni oka
    n = normalize(V * M * normal); // Wektor normalny w przestrzeni oka

    iTexCoord = texCoord;
    iTexCoord1 = (n.xy + 1) / 2;

    ic = color;

    gl_Position = P * V * M * vertex;
}
