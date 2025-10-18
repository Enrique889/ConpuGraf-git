#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Struct para el material del modelo (usa los samplers de la clase Model)
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 

// Struct para definir las propiedades de una fuente de luz
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_LIGHTS 2 // Definimos que vamos a usar 2 luces
uniform Light lights[NR_LIGHTS];
uniform Material material;
uniform vec3 viewPos;

// Función para calcular la contribución de una sola luz
vec3 CalcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = vec3(0.0); // Empezamos con color negro
    
    // Sumamos la contribución de cada luz
    for(int i = 0; i < NR_LIGHTS; i++)
        result += CalcLight(lights[i], norm, FragPos, viewDir);
    
    FragColor = vec4(result, 1.0);
}

// Implementación de la función de cálculo de luz
vec3 CalcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Ambient
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
  	
    // Diffuse 
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;  
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;
        
    return (ambient + diffuse + specular);
}