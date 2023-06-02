#version 330

// Attributs de sommet
layout(location = 0) in vec3 aVertexPosition; 
layout(location = 1) in vec3 aVertexNormal; 
layout(location = 2) in vec2 aVertexTexCoords; 

// Matrices de transformation reçues en uniform
uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;
uniform vec3 uColor;

// Sorties du shader
out vec3 vPosition_vs; 
out vec3 vNormal_vs;
out vec2 vTexCoords; 
out vec3 vColor; 

void main() {
    // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1.);
    vec4 vertexNormal = vec4(aVertexNormal, 0.);

    // Calcul des valeurs de sortie
    vPosition_vs = vec3(uMVMatrix * vertexPosition);
    vNormal_vs = vec3(uNormalMatrix * vertexNormal);
    vTexCoords = aVertexTexCoords;
    vColor = uColor;
    
    // Calcul de la position projetée
    gl_Position = uMVPMatrix * vertexPosition;
}
