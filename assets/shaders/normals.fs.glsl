#version 330

in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec4 fFragColor;

void main()
{
     // Normalisation de la normale
    vec3 normal = normalize(vNormal_vs);

    // Utilisation de la normale comme couleur du fragment
    fFragColor = vec4(normal*0.5+0.5, 1.0);
}