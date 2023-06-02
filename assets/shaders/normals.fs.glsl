#version 330

in vec3 vPosition_vs; 
in vec3 vNormal_vs; 
in vec3 vColor; 

uniform vec3 uMaterialAmbient;
uniform vec3 uMaterialDiffuse;
uniform vec3 uMaterialSpecular;
uniform float uMaterialShininess;
uniform int uUseShadow;

uniform vec3 uSecondLightPosition;  
uniform vec3 uSecondLightColor;     

out vec4 fFragColor;

void main()
{
    if(uUseShadow == 1)
    {
        vec3 ambientColor = uMaterialAmbient;
        vec3 diffuseColor = uMaterialDiffuse;
        vec3 specularColor = uMaterialSpecular;

        // Calcul de l'éclairage
        vec3 lightPosition = vec3(0.0, 4.0, 4.0);
        vec3 lightDirection = normalize(lightPosition - vPosition_vs);
        float ambientStrength = 0.01;
        vec3 ambient = ambientStrength * ambientColor;

        vec3 normal = normalize(vNormal_vs);

        float diffuseStrength = max(dot(normal, lightDirection), 0.0);
        vec3 diffuse = diffuseStrength * diffuseColor;

        vec3 viewDirection = normalize(-vPosition_vs);
        vec3 reflectDirection = reflect(-lightDirection, normal);
        float specularStrength = pow(max(dot(viewDirection, reflectDirection), 0.0), uMaterialShininess);
        vec3 specular = specularStrength * specularColor;

        // Calcul de l'éclairage de la seconde lumière
        vec3 secondLightDirection = normalize(uSecondLightPosition - vPosition_vs);
        float secondDiffuseStrength = max(dot(normal, secondLightDirection), 0.0);
        vec3 secondDiffuse = secondDiffuseStrength * uSecondLightColor;

        vec3 lightingColor = ambient + diffuse + specular + secondDiffuse;

        fFragColor = vec4(lightingColor, 1.0);
    }
    else
    {
        // Utilisation de la normale comme couleur du fragment
        vec3 normal = normalize(vNormal_vs);
        if(vColor.x == 0 && vColor.y == 0 && vColor.z == 0)
        {
            fFragColor = vec4(normal*0.5+0.5, 1.0);
        }
        else
        {
            fFragColor = vec4(vColor.x, vColor.y, vColor.z, 1.0);
        }
    }  
}