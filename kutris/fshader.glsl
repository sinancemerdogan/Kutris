#version 410

in  vec3 fN;
in  vec3 fL;
in  vec3 fV;
in vec4 color;
out vec4 fcolor;
in  vec2 texCoord;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform int UsePicking;
uniform float Shininess;
uniform sampler2D tex;
//Uniform variable for controlling options
uniform int ShadingOption;
uniform int TextureOption;

void main()
{
    if(TextureOption == 0){
        if(UsePicking == 0){
            fcolor = vec4(1.0, 0.0, 0.0, 1.0);
        }
        else if(UsePicking == 1){
            fcolor = vec4(0.0, 1.0, 0.0, 1.0);
        }
        else if(UsePicking == 2){
            fcolor = vec4(0.0, 0.0, 1.0, 1.0);
        }
        else if(UsePicking == 3){
            fcolor = vec4(1.0, 1.0, 0.0, 1.0);
        }
        //ShadingOption = Gouraud
        else if(ShadingOption == 0 && UsePicking == 4) {
            fcolor = color;
        }
    }
    //ShadingOption = Phong
    else if(ShadingOption == 1 && UsePicking == 4 && TextureOption == 0) {
        
        // Normalize the input lighting vectors
        vec3 N = normalize(fN);
        vec3 V = normalize(fV);
        vec3 L = normalize(fL);
        
        // Normalize the input lighting vectors
        vec3 R = normalize( (2.0 * dot(L, N) * N) - L) ; // other phong
        vec3 H = normalize( L + V );
        
        vec4 ambient = AmbientProduct;
        
        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd*DiffuseProduct;
        
        float Ks = pow(max(dot(V, R), 0.0), Shininess); // other phong
        vec4 specular = Ks*SpecularProduct;
        
        // discard the specular highlight if the light's behind the vertex
        if( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }
        
        fcolor = color + diffuse + ambient;
        fcolor.a = 1.0;
        
    }
    else if (TextureOption == 1){
        fcolor = texture(tex, texCoord);
    } else {
        fcolor = texture(tex, texCoord);
    }
    
}

