#version 410
in  vec4 vPosition;
in  vec3 vNormal;
in  vec4 vColor;
in  vec2 vTexCoord;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform float Shininess;

out  vec3 fN;
out  vec3 fV;
out  vec3 fL;
out vec4 color;
out vec2 texCoord;

//Uniform variable for controlling options
uniform int ShadingOption;
uniform int LightSourcePosition;
uniform int ComponentOption;
uniform int TextureOption;


void main()
{
    
    //ShadingOption = Gouraud
    if(ShadingOption == 0 && TextureOption == 0) {
        
        vec3 L, V, H, N;
        // Transform vertex position into camera (eye) coordinates
        vec3 pos = (ModelView * vPosition).xyz;
        
        //LightSourcePosition == Fixed
        if (LightSourcePosition == 0) {
            
            
            L = LightPosition.xyz; // light direction if directional light source
            if( LightPosition.w != 0.0 ) L = LightPosition.xyz - pos;  // if point light source
            
            L = normalize(L);
            
            V = normalize( -pos ); // viewer direction
            H = normalize( L + V ); // halfway vector
            
            // Transform vertex normal into camera coordinates
            N = normalize( ModelView * vec4(vNormal, 0.0) ).xyz;
        }
        //LightSourcePosition == Moving
        else {
            
            L = (ModelView * LightPosition).xyz; // light direction if directional light source
            if( LightPosition.w != 0.0 ) L = (ModelView * LightPosition).xyz - pos;  // if point light source
            
            L = normalize(L);
            
            V = normalize( -pos ); // viewer direction
            H = normalize( L + V ); // halfway vector
            
            // Transform vertex normal into camera coordinates
            N = normalize( ModelView * vec4(vNormal, 0.0) ).xyz;
            
        }
        
        // Compute terms in the illumination equation
        vec4 ambient = AmbientProduct;
        
        float Kd = max( dot(L, N), 0.0 ); //set diffuse to 0 if light is behind the surface point
        vec4  diffuse = Kd * DiffuseProduct;
        
        float Ks = pow( max(dot(N, H), 0.0), Shininess );
        vec4  specular = Ks * SpecularProduct;
        
        //ignore also specular component if light is behind the surface point
        if( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }
        
        gl_Position = Projection * ModelView * vPosition;
        
        color = vColor + diffuse + specular;
        color.a = 1.0;
    }
    else if (ShadingOption == 2 && TextureOption == 0){
        vec3 L, V, H, N;
        // Transform vertex position into camera (eye) coordinates
        vec3 pos = (ModelView * vPosition).xyz;
        
        //LightSourcePosition == Fixed
        if (LightSourcePosition == 0) {
            
            
            L = LightPosition.xyz; // light direction if directional light source
            if( LightPosition.w != 0.0 ) L = LightPosition.xyz - pos;  // if point light source
            
            L = normalize(L);
            
            V = normalize( -pos ); // viewer direction
            H = normalize( L + V ); // halfway vector
            
            // Transform vertex normal into camera coordinates
            N = normalize( ModelView * vec4(vNormal, 0.0) ).xyz;
        }
        //LightSourcePosition == Moving
        else {
            
            L = (ModelView * LightPosition).xyz; // light direction if directional light source
            if( LightPosition.w != 0.0 ) L = (ModelView * LightPosition).xyz - pos;  // if point light source
            
            L = normalize(L);
            
            V = normalize( -pos ); // viewer direction
            H = normalize( L + V ); // halfway vector
            
            // Transform vertex normal into camera coordinates
            N = normalize( ModelView * vec4(vNormal, 0.0) ).xyz;
            
        }
        
        // Compute terms in the illumination equation
        vec4 ambient = AmbientProduct;
        
        float Kd = max( dot(L, N), 0.0 ); //set diffuse to 0 if light is behind the surface point
        vec4  diffuse = Kd * DiffuseProduct;
        
        float Ks = pow( max(dot(N, H), 0.0), Shininess );
        vec4  specular = Ks * SpecularProduct;
        
        //ignore also specular component if light is behind the surface point
        if( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }
        
        gl_Position = Projection * ModelView * vPosition;
        
        color = vColor * 4 *( diffuse + ambient + specular);
        color.a = 1.0;
    }
    //ShadingOption = Phong
    else if(ShadingOption == 1 && TextureOption == 0) {
        
        // Transform vertex position into camera (eye) coordinates
        vec3 pos = (ModelView * vPosition).xyz;
        
        //LightSourcePosition == Fixed
        if (LightSourcePosition == 0) {
            
            fN = (ModelView * vec4(vNormal, 0.0)).xyz; // normal direction in camera coordinates
            
            fV = -pos; //viewer direction in camera coordinates
            
            fL = LightPosition.xyz; // light direction
            
            if( LightPosition.w != 0.0 ) {
                fL = LightPosition.xyz - pos;  //point light source
            }
            
            gl_Position =  Projection * ModelView * vPosition;
        }
        //LightSourcePosition == Moving
        else {
            
            fN = (ModelView * vec4(vNormal, 0.0)).xyz; // normal direction in camera coordinates
            
            fV = -pos; //viewer direction in camera coordinates
            
            fL = (ModelView * LightPosition).xyz; // light direction
            
            if( LightPosition.w != 0.0 ) {
                fL = LightPosition.xyz - pos;  //point light source
            }
            
            gl_Position = Projection * ModelView * vPosition;
            
        }
    } else if (TextureOption == 1){
        color       = vColor;
        texCoord    = vTexCoord;
        gl_Position = Projection * ModelView * vPosition;
    }
    else {
        color       = vColor;
        texCoord    = vTexCoord;
        gl_Position = Projection * ModelView * vPosition;
    }
}
