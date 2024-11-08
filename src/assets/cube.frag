#version 330 core
in vec4 vertexColor;
in vec2 textureCoords;
in vec3 normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float shininess;


void main()
{


    // ambient
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor;

    // specular w/ blinn-phong
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular);

    vec4 textureColor = texture(uTexture, textureCoords) * vec4(result,1.0);
    if(textureColor.a < 0.1)
        discard;
    FragColor = textureColor;
}