//
//  Display a rotating cube around fixed coordinate axes
//
#define GL_SILENCE_DEPRECATION
#include "Angel.h"

typedef vec4  color4;
typedef vec4  point4;

const int sizeOfgame = 10;
const int beginningX = 4;
const int endX = 6;
const int beginningY = 4;
const int endY= 6;
const int totalCubeRequired = 100 + (sizeOfgame*sizeOfgame)*2 ;
mat4 globalModelView;
mat4  modelViews[totalCubeRequired + 101];
const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
int gameEnded = 0;
int blocktoErase = 5;
int patternToDraw = 0;
int score = 0;
int numberOfonebyoneBlock = 10;
int numberOfonebytwoBlock = 10;
int numberOfLBlock = 10;
int numberOftwobytwoBlock = 10;

//------------- texture ------------------
int textureType = 0;
// for texture mapping
GLubyte* imageHeart;
GLubyte* imageBrick;
GLubyte* imageGame;

const int  sizeXHeart = 360;
const int sizeYHeart = 360;
GLubyte heart[sizeXHeart][sizeYHeart][3];

const int  sizeXBrick = 800;
const int sizeYBrick = 800;
GLubyte brick[sizeXBrick][sizeYBrick][3];

const int  sizeXGame = 500;
const int sizeYGame = 500;
GLubyte game[sizeXGame][sizeYGame][3];

vec2    tex_coords[NumVertices];
GLuint textures[3];

//----------------------------------------------


point4 points[totalCubeRequired + 101][NumVertices]; // 10 * 4 + 10 * 3 + 10 * 2  + 10 *1 cubes
color4 colors[totalCubeRequired + 101][NumVertices];
vec3   normals[totalCubeRequired + 101][NumVertices];

GLint degress = 0;
GLfloat counterCLock = 1.0;

//Shading options
enum {Gouraud = 0, Phong = 1};

//Light Source Position
enum {Fixed = 0, Moving = 1};

//Material Options
enum {Plastic = 5, Metallic = 10, Thirty = 20, Hundred = 100};

//Component Options
enum {Specular = 0, Diffuse = 1, Ambient = 2, All = 3};

int shadingOption = Gouraud;
int lightSourcePosition = Fixed;
int materialOption = Plastic;
int componentOption = All;
int pickingOption = 4;


// Vertices of a unit cube centered at origin, sides aligned with axes
point4 twoBytwo[4][8] = {
    {
        //1
        point4( -1.1, -1.0, 0.0, 1 ),
        point4( -1.1, 0.0, 0.0, 1 ),
        point4( -0.1, 0.0, 0.0, 1 ),
        point4( -0.1, -1.0, 0.0, 1 ),
        point4( -1.1, -1.0, -1.0, 1 ),
        point4( -1.1, 0.0, -1.0, 1 ),
        point4( -0.1, 0.0, -1.0, 1 ),
        point4( -0.1, -1.0, -1.0, 1 ),
        
    },
    
    {
        //2
        point4( -1.1, 0.1, 0.0, 1 ),
        point4( -1.1, 1.1, 0.0, 1 ),
        point4( -0.1, 1.1, 0.0, 1 ),
        point4( -0.1, 0.1, 0.0, 1 ),
        point4( -1.1, 0.1, -1.0, 1 ),
        point4( -1.1, 1.1, -1.0, 1 ),
        point4( -0.1, 1.1, -1.0, 1 ),
        point4( -0.1, 0.1, -1.0, 1 ),
        
    },
    
    
    
    { // 3
        point4( 0.0, 0.1, 0.0, 1 ),
        point4( 0.0, 1.1, 0.0, 1 ),
        point4( 1.0, 1.1, 0.0, 1 ),
        point4( 1.0, 0.1, 0.0, 1 ),
        point4( 0.0, 0.1, -1.0, 1 ),
        point4( 0.0, 1.1, -1.0, 1 ),
        point4( 1.0, 1.1, -1.0, 1 ),
        point4( 1.0, 0.1, -1.0, 1 ),
        
    },
    
    {  //4
        point4( 0.0, -1.0, 0.0, 1 ),
        point4( 0.0, 0.0, 0.0, 1 ),
        point4( 1.0, 0.0, 0.0, 1 ),
        point4( 1.0, -1.0, 0.0, 1 ),
        point4( 0.0, -1.0, -1.0, 1 ),
        point4( 0.0, 0.0, -1.0, 1 ),
        point4( 1.0, 0.0, -1.0, 1 ),
        point4( 1.0, -1.0, -1.0, 1 ),
        
    },
};


point4 cubeL[3][8] = {
    {
        //1
        point4( -1.1, -1.0, 0.0, 1 ),
        point4( -1.1, 0.0, 0.0, 1 ),
        point4( -0.1, 0.0, 0.0, 1 ),
        point4( -0.1, -1.0, 0.0, 1 ),
        point4( -1.1, -1.0, -1.0, 1 ),
        point4( -1.1, 0.0, -1.0, 1 ),
        point4( -0.1, 0.0, -1.0, 1 ),
        point4( -0.1, -1.0, -1.0, 1 ),
        
    },
    
    {
        //2
        point4( -1.1, 0.1, 0.0, 1 ),
        point4( -1.1, 1.1, 0.0, 1 ),
        point4( -0.1, 1.1, 0.0, 1 ),
        point4( -0.1, 0.1, 0.0, 1 ),
        point4( -1.1, 0.1, -1.0, 1 ),
        point4( -1.1, 1.1, -1.0, 1 ),
        point4( -0.1, 1.1, -1.0, 1 ),
        point4( -0.1, 0.1, -1.0, 1 ),
        
    },
    
    
    
    {  //4
        point4( 0.0, -1.0, 0.0, 1 ),
        point4( 0.0, 0.0, 0.0, 1 ),
        point4( 1.0, 0.0, 0.0, 1 ),
        point4( 1.0, -1.0, 0.0, 1 ),
        point4( 0.0, -1.0, -1.0, 1 ),
        point4( 0.0, 0.0, -1.0, 1 ),
        point4( 1.0, 0.0, -1.0, 1 ),
        point4( 1.0, -1.0, -1.0, 1 ),
        
    },
};

point4 oneByTwo[2][8] = {
    
    {
        //1
        point4( -1.1, -1.0, 0.0, 1 ),
        point4( -1.1, 0.0, 0.0, 1 ),
        point4( -0.1, 0.0, 0.0, 1 ),
        point4( -0.1, -1.0, 0.0, 1 ),
        point4( -1.1, -1.0, -1.0, 1 ),
        point4( -1.1, 0.0, -1.0, 1 ),
        point4( -0.1, 0.0, -1.0, 1 ),
        point4( -0.1, -1.0, -1.0, 1 ),
        
    },
    
    {  //4
        point4( 0.0, -1.0, 0.0, 1 ),
        point4( 0.0, 0.0, 0.0, 1 ),
        point4( 1.0, 0.0, 0.0, 1 ),
        point4( 1.0, -1.0, 0.0, 1 ),
        point4( 0.0, -1.0, -1.0, 1 ),
        point4( 0.0, 0.0, -1.0, 1 ),
        point4( 1.0, 0.0, -1.0, 1 ),
        point4( 1.0, -1.0, -1.0, 1 ),
        
    },
};

point4 oneByone[1][8] = {
    
    {
        //1
        point4( -1.1, -1.0, 0.0, 1 ),
        point4( -1.1, 0.0, 0.0, 1 ),
        point4( -0.1, 0.0, 0.0, 1 ),
        point4( -0.1, -1.0, 0.0, 1 ),
        point4( -1.1, -1.0, -1.0, 1 ),
        point4( -1.1, 0.0, -1.0, 1 ),
        point4( -0.1, 0.0, -1.0, 1 ),
        point4( -0.1, -1.0, -1.0, 1 ),
        
    }
};

point4 vertices_for_line[8] = {
    point4( -1.03, -1.03, 0.03, 1.0 ),
    point4( -1.03, 0.03, 0.03, 1.0 ),
    point4( 0.03, 0.03, 0.03, 1.0 ),
    point4( 0.03, -1.03, 0.03, 1.0 ),
    point4( -1.03, -1.03, -1.03, 1.0 ),
    point4( -1.03, 0.03, -1.03, 1.0 ),
    point4( 0.03, 0.03, -1.03, 1.0 ),
    point4( 0.03, -1.03, -1.03, 1.0 )
};


// RGBA olors
color4 vertex_colors[9] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 ),   // cyan
    color4( 0.5, 0.5, 0.5, 1.0 )
    
};




int globalCube = 0;
int chosen = 0;
GLFWwindow* window;
int turn_degree=0;
int mess=0;
int turn_to=0;
bool turn = false;
bool move = false;
int move_right = 0;
int go_to= 0;
float move_degree = 0;
int problem = 0;
int initialX = (sizeOfgame/2)-1;
int controllerX = initialX;
int controllerY = sizeOfgame;
bool go_down =false;
int gameMatrix[sizeOfgame][sizeOfgame]; // 9 rows 8 colums
int cubeMatrix[2][2];
int newId2 = totalCubeRequired;
int scoreCube = 0;
int vaoMatrix[40][3] = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1},{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
int totalNumberOfBlocks = 0;
float x = -24;
float y = (sizeOfgame/2)+1+18.24;

// ---------------------patterns -------------------------------------
int houseMatrix2[10][10] = {
        {-1,  0,  0,  0, -1, -1,  0,  0,  0, -1},
        {-1,  0,  0,  0, -1, -1,  0,  0,  0, -1},
        {-1,  0,  0,  0, -1, -1,  0,  0,  0, -1},
        {-1,  0,  0,  0,  0,  0,  0,  0,  0, -1},
        {-1,  0, -1, -1,  0,  0, -1, -1,  0, -1},
        {-1,  0, -1, -1,  0,  0, -1, -1,  0, -1},
        {-1,  0,  0,  0,  0,  0,  0,  0,  0, -1},
        {-1, -1,  0,  0,  0,  0,  0,  0, -1, -1},
        {-1, -1, -1,  0,  0,  0,  0, -1, -1, -1},
        {-1, -1, -1, -1,  0,  0, -1, -1, -1, -1}
};

int hearthMatrix2[10][10] = {
        {-1, -1, -1, -1,  0,  0, -1, -1, -1, -1},
        {-1, -1, -1,  0,  0,  0,  0, -1, -1, -1},
        {-1, -1,  0,  0,  0,  0,  0,  0, -1, -1},
        {-1,  0,  0,  0,  0,  0,  0,  0,  0, -1},
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        { 0,  0,  0,  0, -1, -1,  0,  0,  0,  0},
        {-1,  0,  0, -1, -1, -1, -1,  0,  0, -1}
};

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
enum { frontColor = 0, leftColor = 1, backColor = 2, rightColor = 3, upColor = 4, downColor = 5 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };


// quad generates two triangles for each face and assigns colors to the vertices
int Index = 0;

void quad( int a, int b, int c, int d, int i, int j, color4 colorCode, point4 array[][8])
{

    vec4 u = array[j][b] - array[j][a];
    vec4 v = array[j][c] - array[j][b];
    
    vec3 normal = normalize( cross(u, v) );
    
    colors[i][Index] = colorCode; normals[i][Index] = normal; points[i][Index] = array[j][a]; tex_coords[Index] = vec2( 0.0, 0.0 ); Index++;
    colors[i][Index] = colorCode; normals[i][Index] = normal; points[i][Index] = array[j][b]; tex_coords[Index] = vec2( 0.0, 1.0 ); Index++;
    colors[i][Index] = colorCode; normals[i][Index] = normal; points[i][Index] = array[j][c]; tex_coords[Index] = vec2( 1.0, 1.0 ); Index++;
    colors[i][Index] = colorCode; normals[i][Index] = normal; points[i][Index] = array[j][a]; tex_coords[Index] = vec2( 0.0, 0.0 ); Index++;
    colors[i][Index] = colorCode; normals[i][Index] = normal; points[i][Index] = array[j][c]; tex_coords[Index] = vec2( 1.0, 1.0 ); Index++;
    colors[i][Index] = colorCode; normals[i][Index] = normal; points[i][Index] = array[j][d]; tex_coords[Index] = vec2( 1.0, 0.0 ); Index++;
}

//----------------------------------------------------------------------------
// to create the grid lines
void quadframe( int a, int b, int c, int d, vec3 v, int i)
{
    colors[i][Index] = color4( 1.0, 1.0, 1.0, 1.0 ); points[i][Index] = Translate(v.x, v.y, v.z)* vertices_for_line[a]; Index++;
    colors[i][Index] = color4( 1.0, 1.0, 1.0, 1.0 ); points[i][Index] = Translate(v.x, v.y, v.z)* vertices_for_line[b]; Index++;
    colors[i][Index] = color4( 1.0, 1.0, 1.0, 1.0 ); points[i][Index] = Translate(v.x, v.y, v.z)* vertices_for_line[b]; Index++;
    colors[i][Index] = color4( 1.0, 1.0, 1.0, 1.0 ); points[i][Index] = Translate(v.x, v.y, v.z)* vertices_for_line[c]; Index++;
    colors[i][Index] = color4( 1.0, 1.0, 1.0, 1.0 ); points[i][Index] = Translate(v.x, v.y, v.z)* vertices_for_line[c]; Index++;
    colors[i][Index] = color4( 1.0, 1.0, 1.0, 1.0 ); points[i][Index] = Translate(v.x, v.y, v.z)* vertices_for_line[d]; Index++;
    colors[i][Index] = color4( 1.0, 1.0, 1.0, 1.0 ); points[i][Index] = Translate(v.x, v.y, v.z)* vertices_for_line[d]; Index++;
    colors[i][Index] = color4( 1.0, 1.0, 1.0, 1.0 ); points[i][Index] = Translate(v.x, v.y, v.z)* vertices_for_line[a]; Index++;
}

// generate 12 triangles: 36 vertices and 36 colors
color4 frontC = vertex_colors[4];
color4 otherC = vertex_colors[6];
void colorcube(int i, int j, point4 a[][8])
{
   quad( 1, 0, 3, 2, i, j, frontC, a);
   quad( 2, 3, 7, 6, i, j, otherC, a);
   quad( 3, 0, 4, 7, i, j, otherC, a);
   quad( 6, 5, 1, 2, i, j, otherC, a);
   quad( 4, 5, 6, 7, i, j, otherC, a);
   quad( 5, 4, 0, 1, i, j, otherC, a);
   Index = 0;
    
}
void frames(vec3 v, int i){
    // to create the lines of the grid
    quadframe( 0, 1, 2, 3, v, i);
    quadframe( 4, 5, 6, 7, v, i);
    quadframe( 0, 4, 7, 3, v, i);
    quadframe( 1, 5, 6, 2, v, i);
    Index = 0;
}



// Model-view and projection matrices uniform location
GLuint  program,ModelView, Projection;

// Initialize shader lighting parameters
point4 light_position(0.0, 2.0, 4.0, 1.0 ); //directional light source
color4 light_ambient( 0.2, 0.2, 0.2, 1.0 ); // L_a
color4 light_diffuse( 0.2, 1.0, 1.0, 1.0 ); // L_d
color4 light_specular( 1.0, 1.0, 1.0, 1.0 ); // L_s

color4 material_ambient( 1.0, 0.0, 1.0, 1.0 ); // k_a
color4 material_diffuse( 1.0, 0.8, 0.0, 1.0 ); // k_d
color4 material_specular( 1.0, 0.8, 0.0, 1.0 ); // k_s
float  material_shininess = Hundred;

color4 ambient_product = light_ambient * material_ambient;
color4 diffuse_product = light_diffuse * material_diffuse;
color4 specular_product = light_specular * material_specular;

GLuint ShadingOption, LightSourcePosition, ComponentOption, UsePicking, TextureOption;

// to read the ppm
GLubyte* readAndInitPpm(const char* imageName) {
    int n, m;
    FILE* fd;
    int k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;
    fd = fopen(imageName, "r");
    fscanf(fd, "%[^\n]", b);
    if (b[0] != 'P' || b[1] != '3') {
        printf("%s is not a PPM file!\n",b);
        exit(0);
    }
    printf("%s is a PPM file\n",b);
    fscanf(fd, "%c", &c);
    while (c == '#')
    {
        fscanf(fd, "%[^\n]",b);
        printf("%s\n", b);
        fscanf(fd, "%c", &c);
    }
    ungetc(c, fd);

    fscanf(fd, "%d %d %d", &n, &m, &k);
    printf("%d rows %d columns max value= %d\n", n, m, k);
    nm = n * m;
    GLubyte* image = (GLubyte*) malloc(3 * sizeof(GLuint) * nm);
    for (i = nm; i > 0; i--)
    {
        fscanf(fd, "%d %d %d", &red, &green, &blue);
        image[3 * nm - 3 * i] = red;
        image[3 * nm - 3 * i + 1] = green;
        image[3 * nm - 3 * i + 2] = blue;
    }
    return image;
    
}
//---------------------------------------------------------------------
//
// init
//
int numberOfPixel[10] = {12,5,11,11,9,11,12,7,13,12};
void translateWW (int x, int y, int index, int number);
int xScore = 0;
int yScore = 0;

// determine and determine the places of the cubes to create digits
void getScorePixel() { // index 55
    int first = score % 10;
    int second = ((score - first) / 10) % 10;
    int third = (int) score /100 ;
    scoreCube = 55;
    int x = xScore;
    std::cout << x << " " << yScore  << std::endl;
    translateWW(x,yScore, 355, third);
    x += 5;
    translateWW(x,yScore, 355+numberOfPixel[third], second);
    x += 5;
    translateWW(x,yScore, 355+numberOfPixel[third]+numberOfPixel[second], first);
}



int pixels[3][5];

void getNumberPixels(int number);

// to create digits and the word SCORE by translating cubes
void translateWW (int x, int y, int index, int number){
    
    getNumberPixels(number);
    for(int i=0; i<3; i++){
        for(int j=0; j<5; j++){
            if(pixels[i][j] == 0){
                vec3 vtvec = vec3(x+i*1.3, y+j*1.3, 0);
                modelViews[index] = Scale(0.2,0.2,0.2) * RotateX( 10.0 ) * RotateY( 10.0 ) * RotateZ( 0.0 ) * Translate(vtvec);
                index ++;
                newId2++;
                scoreCube++;
                
            }
        }
    }
}

// to create digits and the word SCORE by translating cubes
void translateScore (int x, int y, int index, int number){
    
    getNumberPixels(number);
    for(int i=0; i<3; i++){
        for(int j=0; j<5; j++){
            if(pixels[i][j] == 0){
                vec3 vtvec = vec3(x+i*1.3, y+j*1.3, 0);
                modelViews[index] = modelViews[index] * Translate(vtvec);
                index ++;
                newId2++;
                scoreCube++;
                
            }
        }
    }
}

// create a matrix that will determine the places of the not empty cube places
void getNumberPixels(int number) {
    
    // Initialize all pixels to 0
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            pixels[i][j] = 0;
        }
    }

    // Assign specific pixels for each number
    switch (number) {
        case 0: //0
            pixels[1][1] = -1;
            pixels[1][3] = -1;
            pixels[1][2] = -1;
            break;
        case 1:
            pixels[0][0] = -1;
            pixels[0][1] = -1;
            pixels[0][2] = -1;
            pixels[0][3] = -1;
            pixels[0][4] = -1;
            pixels[2][0] = -1;
            pixels[2][1] = -1;
            pixels[2][2] = -1;
            pixels[2][3] = -1;
            pixels[2][4] = -1;
            break;
        case 2:
            pixels[0][3] = -1;
            pixels[1][3] = -1;
            pixels[2][1] = -1;
            pixels[1][1] = -1;
        
            break;
        case 3:
            pixels[0][3] = -1;
            pixels[1][3] = -1;
            pixels[0][1] = -1;
            pixels[1][1] = -1;
            
            break;
        case 4:
            pixels[0][0] = -1;
            pixels[0][1] = -1;
            pixels[1][0] = -1;
            pixels[1][1] = -1;
            pixels[1][4] = -1;
            pixels[1][3] = -1;
        
            break;
        case 5: //S
            pixels[0][1] = -1;
            pixels[1][1] = -1;
            pixels[2][3] = -1;
            pixels[1][3] = -1;
            break;
        case 6:
            pixels[1][1] = -1;
            pixels[2][3] = -1;
            pixels[1][3] = -1;
    
            break;
        case 7:
            pixels[0][3] = -1;
            pixels[1][3] = -1;
            pixels[0][2] = -1;
            pixels[1][2] = -1;
            pixels[0][1] = -1;
            pixels[1][1] = -1;
            pixels[0][0] = -1;
            pixels[1][0] = -1;
            break;
        case 8:
            pixels[1][1] = -1;
            pixels[1][3] = -1;
            break;
        case 9:
            pixels[0][1] = -1;
            pixels[1][1] = -1;
            pixels[1][3] = -1;
            break;
        case 10: //C
            pixels[1][3] = -1;
            pixels[2][3] = -1;
            pixels[1][2] = -1;
            pixels[2][2] = -1;
            pixels[1][1] = -1;
            pixels[2][1] = -1;
            break;
        case 11: //R
            pixels[1][0] = -1;
            pixels[1][1] = -1;
            pixels[1][3] = -1;
            pixels[2][2] = -1;
            pixels[2][4] = -1;
            break;
        case 12: //E
            pixels[2][1] = -1;
            pixels[1][1] = -1;
            pixels[2][3] = -1;
            pixels[1][3] = -1;
            break;
            
        case 13: //:
            pixels[0][0] = -1;
            pixels[0][1] = -1;
            pixels[0][2] = -1;
            pixels[0][3] = -1;
            pixels[0][4] = -1;
            pixels[2][0] = -1;
            pixels[2][1] = -1;
            pixels[2][2] = -1;
            pixels[2][3] = -1;
            pixels[2][4] = -1;
            pixels[1][0] = -1;
            pixels[1][2] = -1;
            pixels[1][4] = -1;
            break;
            
        default:
            break;
    }

}

GLuint vaoArray[totalCubeRequired + 101];
GLuint buffer;
GLint Color;
mat4  projection = Ortho(-2.5, 2.5, -2.5, 2.5, -2.5, 2.5);

void
init()
{
        
    gameEnded = 0;
    blocktoErase = 5;
    patternToDraw = 0;
    score = 0;
    numberOfonebyoneBlock = 10;
    numberOfonebytwoBlock = 10;
    numberOfLBlock = 10;
    numberOftwobytwoBlock = 10;
    totalNumberOfBlocks = 0;
    
    
    
    
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        // Generate a random number either 0 or 1
        patternToDraw = std::rand() % 2;
        // ------- texture -----------------
        imageBrick = readAndInitPpm("oldBrick.ppm");
        imageHeart = readAndInitPpm("kalp2.ppm");
        imageGame= readAndInitPpm("play.ppm");
    
        glGenTextures( 3, textures );
    
        // for Heart texture
        glBindTexture( GL_TEXTURE_2D, textures[0] );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, sizeXHeart, sizeYHeart, 0,
                  GL_RGB, GL_UNSIGNED_BYTE, imageHeart );
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
        // for Brick texture
        glBindTexture( GL_TEXTURE_2D, textures[1] );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, sizeXBrick, sizeYBrick, 0,
                  GL_RGB, GL_UNSIGNED_BYTE, imageBrick );
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        // for Brick texture
       glBindTexture( GL_TEXTURE_2D, textures[2] );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, sizeXGame, sizeYGame, 0,
                  GL_RGB, GL_UNSIGNED_BYTE, imageGame );
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // --------------------------------------------
    
        GLuint vPosition;
        GLuint vColor;
        GLuint vNormal;
        
        // one by one 10*1
        for(int i = 0; i < 10; i++){
            int j = 0 ;
            colorcube(i,j,oneByone);
        }
        // one by two 10*2
        for(int i = 10; i < 30; i++){
            int j = i%2;
            colorcube(i,j,oneByTwo);
        }
        // L block 10*3
        for(int i = 30; i < 60; i++){
            int j = i%3;
            colorcube(i,j,cubeL);
        }
        // two by two 10*4
        for(int i = 60; i < 100; i++){
            int j = i%4;
            colorcube(i,j,twoBytwo);
        }
        int newI = 100;
        for(int x=-beginningX;x<endX;x++){
            for(int j=-beginningY;j<endY;j++){
                vec3 v = vec3(x, j, 0);
                frames(v, newI);
                newI ++;
            }
        }
    
    int indexX = 0;
    int indexY = 0;
    
    // to color the patterns with magenta (not empty spots) and black (empty spots)
    for(int i = totalCubeRequired-(sizeOfgame*sizeOfgame); i < totalCubeRequired; i++){
        int j = 0;
        
        indexX = (i-(totalCubeRequired-(sizeOfgame*sizeOfgame)))%sizeOfgame;
        indexY = (i-(totalCubeRequired-(sizeOfgame*sizeOfgame)))/sizeOfgame ;
        if(hearthMatrix2[indexX][indexY] == 0 && patternToDraw == 0){
            //not empty
            frontC = vertex_colors[5];
            otherC = vertex_colors[0];
        }
        else if(houseMatrix2[indexX][indexY] == 0 && patternToDraw == 1){
            // not empty
            frontC = vertex_colors[5];
            otherC = vertex_colors[0];
        }
        else {
            // empty
            frontC = vertex_colors[0];
            otherC = vertex_colors[0];
        }
        colorcube(i,j,twoBytwo);
        
        
    }
    frontC = vertex_colors[4];
    otherC = vertex_colors[6];
    
    // initialize the game area matrix to be all -1
        for (int i = 0; i<sizeOfgame ; i++){
            for (int j = 0; j<sizeOfgame ; j++){
                gameMatrix[i][j] = -1;
            }
        }
        
        for (int i = totalCubeRequired; i<totalCubeRequired+101; i++){
            int j = 0;
            colorcube(i,j,twoBytwo);
        }

        GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
        const vec3 displacement( 0.0, 0.0, 0.0 );
        
        // Create a vertex array object
        glGenVertexArrays( totalCubeRequired+101, vaoArray );
        for (int i = 0; i < totalCubeRequired+101; i++) {
            
            glBindVertexArray( vaoArray[i] );
            glGenBuffers( 1, &buffer );
            glBindBuffer( GL_ARRAY_BUFFER, buffer );
            glBufferData( GL_ARRAY_BUFFER, sizeof(points[i]) + sizeof(colors[i]) + sizeof(normals[i])  + sizeof(tex_coords), NULL, GL_STATIC_DRAW );
            glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points[i]), points[i] );
            glBufferSubData( GL_ARRAY_BUFFER, sizeof(points[i]), sizeof(normals[i]), normals[i] );
            glBufferSubData( GL_ARRAY_BUFFER, sizeof(points[i]) + sizeof(normals[i]), sizeof(colors[i]), colors[i] );
            glBufferSubData( GL_ARRAY_BUFFER, sizeof(points[i]) + sizeof(normals[i]) + sizeof(colors[i]), sizeof(tex_coords), tex_coords );
            
            vPosition = glGetAttribLocation( program, "vPosition" );
            glEnableVertexAttribArray( vPosition );
            glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
            
            vNormal = glGetAttribLocation( program, "vNormal" );
            glEnableVertexAttribArray( vNormal );
            glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points[i])) );
            
            vColor = glGetAttribLocation( program, "vColor" );
            glEnableVertexAttribArray( vColor );
            glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points[i]) + sizeof(normals[i])));
            
            GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
            glEnableVertexAttribArray(vTexCoord);
            glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                   BUFFER_OFFSET(sizeof(points[i]) + sizeof(normals[i]) + sizeof(colors[i])));
            
            globalModelView = RotateX( 10.0 ) * RotateY( 10.0 ) * RotateZ( 0.0 ) ;
      
            modelViews[i] =  Scale(0.2,0.2,0.2) * RotateX( 10.0 ) * RotateY( 10.0 ) * RotateZ( 0.0 );
            
            if (i<100){
                modelViews[i] = modelViews[i] * Translate(0, (sizeOfgame/2)+1+0.24, 0);
            }
        }
    
    // create the pattern cubes
        int newId = totalCubeRequired-(sizeOfgame*sizeOfgame);
            for(int x=-beginningX;x<endX;x++){
                for(int j=-beginningY;j<endY;j++){
                    vec3 v = vec3(x+0.2, j-0.1, -1.2);
                    modelViews[newId] = modelViews[newId] * Translate(v);
                    newId ++;
                }
            }
        for(int i=0 ; i<totalCubeRequired; i++){
            modelViews[i] = modelViews[i] * Translate(0,-2,0);
        }
    x = -22;
    y = (sizeOfgame/2)+1+17.24;
    scoreCube = 0;
    newId2 = totalCubeRequired;
        // S
        translateWW(x,y,newId2,5);
        
        //C
        x += 5;
        translateWW(x,y,newId2,10);
    
        //O
        x += 5;
        translateWW(x,y,newId2,0);
        //R
        x += 5;
        translateWW(x,y,newId2,11);

        //E
        x += 5;
        translateWW(x,y,newId2,12);
        
        //:
        x += 5;
        translateWW(x,y,newId2,13);
        
        x += 5;
        xScore = x;
        yScore = y;
         //0
        translateWW(x,y,newId2,0);
        //0
        x += 5;
        translateWW(x,y,newId2,0);
        //0
        x += 5;
        translateWW(x,y,newId2,0);
    
        glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
                     1, ambient_product );
        glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
                     1, diffuse_product );
        glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
                     1, specular_product );
        
        glUniform4fv( glGetUniformLocation(program, "LightPosition"),
                     1, light_position);
        
        glUniform1f( glGetUniformLocation(program, "Shininess"),
                    material_shininess );
    
        
        
        ShadingOption = glGetUniformLocation(program, "ShadingOption");
        UsePicking = glGetUniformLocation(program, "UsePicking");
        LightSourcePosition = glGetUniformLocation(program, "LightSourcePosition");
        ComponentOption = glGetUniformLocation(program, "ComponentOption");
        TextureOption = glGetUniformLocation(program, "TextureOption");
    
        glUniform1f( ShadingOption, shadingOption );
        glUniform1i(UsePicking, pickingOption);
        glUniform1f( LightSourcePosition, lightSourcePosition );
        glUniform1f( ComponentOption, componentOption );
        glUniform1i( TextureOption, 0 );
        
        // Retrieve uniform variable locations
        ModelView = glGetUniformLocation( program, "ModelView" );
        Projection = glGetUniformLocation( program, "Projection" );
    
    
        
        glBindTexture(GL_TEXTURE_2D, textures[patternToDraw]);
        // Enable hiddden surface removal
        glEnable( GL_DEPTH_TEST );
        
        //projection = Ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
         // Ortho(): user-defined function in mat.h
    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
        // Set state variable "clear color" to clear buffer with.
        glClearColor( 0.0, 0.0, 0.0, 1.0 );
    //modelViews[totalCubeRequired+101] = modelViews[totalCubeRequired+101] * Translate(0,0,0);
}


//---------------------------------------------------------------------
//
// display
//
int forceFall = 1;
int createNew = 0;
enum { onebyoneBlock = 1, onebytwoBlock = 2, LBlock = 3, twobytwoBlock = 4 };


int blockToDraw = 0;
int fromVao = 0;
bool new_obj=false;
int new_pos = 1;
int oneTimeToRight = 1;
int twoTimeToRight = 1;
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int checkIfGameEnded (){
    // we check if the number of block ended
    if(numberOfonebyoneBlock == 0 && numberOfonebytwoBlock == 0 && numberOfLBlock == 0 && numberOftwobytwoBlock == 0){
        std::cout << "Out of block" << std::endl;
       return 1;
    }
    int full = 0;
    for (int i = 0; i<sizeOfgame ; i++){
        // see if the top of the game area is full
        if(gameMatrix[sizeOfgame-1][i] != -1){
            full ++;
        }
    }
    if (full == 12) {
        return  1;
    }
    
    return 0;
}

// check and assign which randomly selected block to draw
void checkNumberOfBlocksLeft(int random_number) {
    if (random_number == 1 && numberOfonebyoneBlock != 0){
        std::cout << "onebyoneBlock" << std::endl;
        numberOfonebyoneBlock --;
        totalNumberOfBlocks ++;
        blockToDraw = onebyoneBlock;
    }
    else if (random_number == 2 && numberOfonebytwoBlock != 0){
        std::cout << "onebytwoBlock" << std::endl;
        numberOfonebytwoBlock --;
        totalNumberOfBlocks ++;
        blockToDraw = onebytwoBlock;
    }
    else if (random_number == 3 && numberOfLBlock != 0){
        std::cout << "LBlock" << std::endl;
        numberOfLBlock --;
        totalNumberOfBlocks ++;
        blockToDraw = LBlock;
    }
    else if (random_number == 4 &&  numberOftwobytwoBlock != 0){
        std::cout << "twobytwoBlock" << std::endl;
        numberOftwobytwoBlock --;
        totalNumberOfBlocks ++;
        blockToDraw = twobytwoBlock;
    }
    else {
        if(numberOfonebyoneBlock > 0 || numberOfonebytwoBlock > 0 || numberOfLBlock > 0 || numberOftwobytwoBlock >0){
            std::cout << "checkBLOCK" << std::endl;
            
            checkNumberOfBlocksLeft((random_number+1) % 4);
        }
        
    }
}

// turn the cube 90 degrees clockwise
mat4 turn_cube(mat4 model_view, int blockToDraw){
    
    if (turn_to == 4){
        turn_degree++;
        if (turn_degree==45* blockToDraw){turn=false; turn_degree=0; mess=0;}
        
        return model_view =  model_view * RotateZ(-2);
        //return model_view = model_view * RotateZ( -2 );
    }
    if (turn_to == 5){
        turn_degree++;
        if (turn_degree==45* blockToDraw){turn=false; turn_degree=0; mess=0;}
        
        return model_view =  model_view * RotateZ(2);
    }
    return 0;
}

// move the cube 1 unit
mat4 move_cube(mat4 model_view, int blockToDraw, int i){
    
    if (go_to == 1){
        if (i == blockToDraw-1){
            move_degree+=0.2;
        }
        if (move_degree == 1 ){move=false; move_degree=0; mess=0;}
        
        if(problem%4==0)
            return model_view =  model_view * Translate(0.212, 0, 0);
        if(problem%4==1)
            return model_view =  model_view * Translate(0, 0.212, 0);
        if(problem%4==2)
            return model_view =  model_view * Translate(-0.212, 0, 0);
        if(problem%4==3)
            return model_view =  model_view * Translate(0, -0.212, 0);
    }
    if (go_to == 2){
        if (i == blockToDraw-1){
            move_degree+=0.2;
        }
        if (move_degree == 1 ){move=false; move_degree=0; mess=0;}
        
        if(problem%4==0)
            return model_view =  model_view * Translate(-0.212, 0, 0);
        if(problem%4==1)
            return model_view =  model_view * Translate(0, -0.212, 0);
        if(problem%4==2)
            return model_view =  model_view * Translate(0.212, 0, 0);
        if(problem%4==3)
            return model_view =  model_view * Translate(0, 0.212, 0);
    }
    
    return 0;
}

// check collisions while making the block fall
bool down_controller(int controllerY){
    for (int i = 0; i<2 ; i++){
        for (int j = 0; j<2; j++){
            if(cubeMatrix[i][j] != -1){
                if(gameMatrix[controllerY+i-1][controllerX+j] != -1 && !(controllerY == sizeOfgame && i==1)){
                    return false;
                }
            }
        }
    }
    if(controllerY-1 == -1){
        return false;
    }
    return true;
}

void updateGameMatrix(){
    for (int i = 0; i<2; i++){
        for (int j = 0; j<2; j++){
            if(cubeMatrix[i][j] != -1){
                gameMatrix[controllerY+i][controllerX+j] = cubeMatrix[i][j];
            }
        }
    }
    vaoMatrix[totalNumberOfBlocks-1][0] = fromVao;
    vaoMatrix[totalNumberOfBlocks-1][1] = blockToDraw;

    
}

// make the object fall while checking if there is collision
mat4 toTheBottom(mat4 model_view, int blockToDraw, int i){
    if (i == blockToDraw-1){
        move_degree+=0.2;
    }
    mat4 model_view_new;
    if(problem%4==0 )
        model_view_new =  model_view * Translate(0, -0.212, 0);
    if(problem%4==1)
        model_view_new =  model_view * Translate(0.212, 0, 0);
    if(problem%4==2)
        model_view_new =  model_view * Translate(0, 0.212, 0);
    if(problem%4==3)
        model_view_new =  model_view * Translate(-0.212, 0, 0);
   
    if(down_controller(controllerY) == true && move_degree == 1){
        move_degree=0;
        controllerY--;
    }
    else if(down_controller(controllerY) == false){
            go_down=false;
            move_degree=0;
            mess=0;
            updateGameMatrix();
        if(i == blockToDraw-1) forceFall = 1;
            return model_view;
    }
    
    return model_view_new;
}

void
display(void)
{
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    const vec3 displacement( 0.0, 0.0, 0.0 );
    
        if(createNew == 1 ){
            srand(time(NULL));
            int random_number = (rand() % 4) + 1;
            createNew = 0;
            checkNumberOfBlocksLeft(random_number);
            
            
        }
    
    if(createNew == 0){
// ----------- calculate the from Vao index of the randomly selected block --------------
        if (blockToDraw == onebyoneBlock) {
            fromVao = 10 - numberOfonebyoneBlock - 1;
        }
        
        else if (blockToDraw == onebytwoBlock) {
            fromVao = 10 + (10 - numberOfonebytwoBlock - 1) * 2 ;
        }
        
        else if (blockToDraw == LBlock) {
            fromVao = 10 + 20 + (10 - numberOfLBlock - 1) * 3 ;
        }
        
        else if (blockToDraw == twobytwoBlock) {
            fromVao = 10 + 20 + 30 + (10 - numberOftwobytwoBlock - 1) * 4 ;
        }
// ---------------rotate the cube---------------------------
        if(turn){
            for(int i = 0; i < blockToDraw; i++){
                modelViews[fromVao+i] = turn_cube(modelViews[fromVao+i], blockToDraw);
                
            }
        }
// ---------------translate the cube---------------------------
        if (move){
            for(int i = 0; i < blockToDraw; i++){
                modelViews[fromVao+i] = move_cube(modelViews[fromVao+i], blockToDraw, i);
                
            }
        }
// ---------------make the cube fall---------------------------
        if (go_down){
            for(int i = 0; i < blockToDraw; i++){
                modelViews[fromVao+i] = toTheBottom(modelViews[fromVao+i], blockToDraw, i);
                
            }
        }
// ---------------initialize the 2x2 block array and assign the vao indexes ---------------------------
        if (new_obj){
            for (int i = 0; i<2 ; i++){
                for (int j = 0; j<2 ; j++){
                    cubeMatrix[i][j] = -1;
                }
            }
            if(blockToDraw == 1){
                cubeMatrix[0][0] = fromVao;
            }
            else if (blockToDraw == 2){
                cubeMatrix[0][0] = fromVao;
                cubeMatrix[0][1] = fromVao+1;
            }
            else if (blockToDraw == 3){
                cubeMatrix[0][0] = fromVao;
                cubeMatrix[1][0] = fromVao+1;
                cubeMatrix[0][1] = fromVao+2;
            }
            else if (blockToDraw == 4){
                cubeMatrix[0][0] = fromVao;
                cubeMatrix[1][0] = fromVao+1;
                cubeMatrix[1][1] = fromVao+2;
                cubeMatrix[0][1] = fromVao+3;
            }
            new_obj=false;
        }
    
// ---------------draw the grid lines---------------------------
        glEnable(GL_LINE_SMOOTH);
        
            for(int a = 100; a<totalCubeRequired-(sizeOfgame*sizeOfgame); a++ ){
                glBindVertexArray( vaoArray[a] );
                glUniformMatrix4fv( ModelView, 1, GL_TRUE,  Scale(1.6, 1.6, 1.6)*modelViews[a]);
                glDrawArrays( GL_LINES, 0, 32 );
            }
// ---------------draw the randomly selected block ---------------------------
       for (int i = 0; i<2 ; i++){
            for (int j = 0; j<2 ; j++){
                if(cubeMatrix[i][j] != -1 && gameEnded != 1) {
                    glBindVertexArray( vaoArray[cubeMatrix[i][j]] );
                    glUniformMatrix4fv( ModelView, 1, GL_TRUE,  Scale(1.5, 1.5, 1.5)*modelViews[cubeMatrix[i][j]]);
                    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
                }
            }
        }
// ---------------draw the patterns---------------------------
            for(int i = totalCubeRequired-(sizeOfgame*sizeOfgame); i < totalCubeRequired; i++){
                glBindVertexArray( vaoArray[i] );
                glUniformMatrix4fv( ModelView, 1, GL_TRUE, Scale(1.6, 1.6, 1.6)*modelViews[i]);
                glDrawArrays( GL_TRIANGLES, 0, NumVertices );
            }
// ---------------draw the SCORE and 3 digits ---------------------------
        for(int i = totalCubeRequired; i < totalCubeRequired+scoreCube; i++){
            glBindVertexArray( vaoArray[i] );
            glUniformMatrix4fv( ModelView, 1, GL_TRUE, Scale(0.4, 0.4, 0.4)*modelViews[i]);
            glDrawArrays( GL_TRIANGLES, 0, NumVertices );
        }
// ---------------draw the PLAY AGAIN button ---------------------------
        if (gameEnded){
            for(int i = totalCubeRequired+scoreCube; i < totalCubeRequired+scoreCube+1; i++){
                glBindVertexArray( vaoArray[i] );
                glBindTexture( GL_TEXTURE_2D, textures[2] );
                glUniform1i(TextureOption, 1);
                glUniformMatrix4fv( ModelView, 1, GL_TRUE, Translate(0.37,1.73,1) * Scale(0.68, 0.68, 0.68)* RotateX( 0 ) * RotateY( 0 ) * RotateZ( 0.0 ));
                glDrawArrays( GL_TRIANGLES, 0, NumVertices );
                glUniform1i(TextureOption, 0);
                glBindTexture( GL_TEXTURE_2D, textures[patternToDraw] );
            }
        }
        
    
//to draw the game area array (all of the blocks that are already been placed)
// and assign texture at the end of the game
        score = 0;
        for(int i = 0; i < sizeOfgame; i++){
            for (int j = 0; j<sizeOfgame; j++){
                if(gameMatrix[i][j] != -1 ){
                    if(gameEnded == 1 && patternToDraw == 0 && hearthMatrix2[i][j] != -1) {
                        glUniform1i(TextureOption, 1);
                    } else if (gameEnded == 1 && patternToDraw == 1 && houseMatrix2[i][j] != -1) {
                        glUniform1i(TextureOption, 1);
                    }
                    if(patternToDraw == 0 && hearthMatrix2[i][j] != -1){
                        score += 1;
                    } else if (patternToDraw == 0 && hearthMatrix2[i][j] == -1){
                        score -= 2;
                    }
                    if(patternToDraw == 1 && houseMatrix2[i][j] != -1){
                        score += 1;
                    } else if (patternToDraw == 1 && houseMatrix2[i][j] == -1){
                        score -= 2;
                    }
                    glBindVertexArray( vaoArray[gameMatrix[i][j]] );
                    glUniformMatrix4fv( ModelView, 1, GL_TRUE, Scale(1.5, 1.5, 1.5)*modelViews[gameMatrix[i][j]]);
                    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
                } else {
                    if(gameEnded == 1) {
                        glUniform1i(TextureOption, 0);
                    }
                }
            }
        }
        // update the score each time
        if(score < 0 ) score = 0;
        getScorePixel();
        
        
    // if the previously place block fall and placed in the game area select a new block type
        if (forceFall == 1 && new_obj == false && gameEnded != 1) {
            if (numberOfonebyoneBlock > 0 || numberOfonebytwoBlock > 0 || numberOfLBlock > 0 || numberOftwobytwoBlock >0){
                std::cout << "New Block" << std::endl;
                problem=0;
                new_obj=true;
                controllerX= initialX;
                controllerY=sizeOfgame;
                forceFall = 0;
                createNew = 1;
            }
        }
    }
}

bool checkFall(){
    for (int i = 0; i<2 ; i++){
        for (int j = 0; j<2; j++){
            if(cubeMatrix[i][j] != -1){
                if(gameMatrix[controllerY+i-2][controllerX+j] != -1 && !(controllerY == sizeOfgame && i==1)){
                    return false;
                }
            }
        }
    }
    return true;
}

// to reshape
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); //may not need this since the default is usually the window size
    //projection = Ortho(-2.5, 2.5, -2.5, 2.5, -2.5, 2.5);
    // Set projection matrix
    
    if (width <= height)
        projection = Ortho(-2.5, 2.5, -2.5 * (GLfloat) height / (GLfloat) width,
                           2.5 * (GLfloat) height / (GLfloat) width, -2.5, 2.5);
    else
        projection = Ortho(-2.5 * (GLfloat) width / (GLfloat) height, 2.5 *
                             (GLfloat) width / (GLfloat) height, -2.5, 2.5, -2.5, 2.5);
    
    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
 
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // rotation = 0 - front  1 - back   2 - right   3 - left
    if(action == GLFW_RELEASE && ((gameEnded != 1) ||  key == GLFW_KEY_I)) {
        switch( key ) {
            case GLFW_KEY_ESCAPE: case GLFW_KEY_Q:
                exit( EXIT_SUCCESS );
                break;
                
            case GLFW_KEY_DOWN: // make the object fall
               if((blockToDraw == 1 &&  gameMatrix[controllerY-1][controllerX] == -1) || ((cubeMatrix[1][0] != -1 || cubeMatrix[1][1]!= -1 ) && checkFall()) || ((cubeMatrix[1][0] == -1 && cubeMatrix[1][1] == -1) &&  gameMatrix[controllerY-1][controllerX] == -1 && gameMatrix[controllerY-1][controllerX+1] == -1)) {
                    if((turn_degree == 0) and (mess==0)){
                        go_down = true;
                        mess++;
                        //forceFall = 1;
                    }
                   oneTimeToRight = 1;
                   twoTimeToRight = 1;
                }
                break;
            
           /* case GLFW_KEY_N:
                if (forceFall == 1 && new_obj == false) {
                    std::cout << "New Block" << std::endl;
                    problem=0;
                    new_obj=true;
                    controllerX=initialX;
                    controllerY=sizeOfgame;
                    forceFall = 0;
                    createNew = 1;
                }
                break;*/
            case GLFW_KEY_SPACE: // to rotate the object
                if((turn_degree == 0) and (mess==0)){
                    if(blockToDraw == 1){
                        
                    }else {
                        mess++;
                        turn_to=4;
                        turn=true;
                    }
                    if(blockToDraw == 3 || blockToDraw == 4){
                        int temp = cubeMatrix[0][0];
                        cubeMatrix[0][0] = cubeMatrix[0][1];
                        cubeMatrix[0][1] = cubeMatrix[1][1];
                        cubeMatrix[1][1] = cubeMatrix[1][0];
                        cubeMatrix[1][0] = temp;
                        problem++;
                    }
                    if (blockToDraw == 2){
                        if(problem == 0){
                            int temp = cubeMatrix[0][0];
                            cubeMatrix[0][0] = cubeMatrix[0][1];
                            cubeMatrix[0][1] = cubeMatrix[1][1];
                            cubeMatrix[1][1] = cubeMatrix[1][0];
                            cubeMatrix[1][0] = temp;
                            problem = 1;
                        } else {
                            int temp = cubeMatrix[1][0];
                            cubeMatrix[1][0] = cubeMatrix[1][1];
                            cubeMatrix[1][1] = cubeMatrix[0][1];
                            cubeMatrix[0][1] = cubeMatrix[0][0];
                            cubeMatrix[0][0] = temp;
                            problem = 0;
                            turn_to = 5;
                        }
                    
                    }
                    
                    
                   
                }
                break;
            case GLFW_KEY_RIGHT: // move the block one unit to the right
                glUniform1f( glGetUniformLocation(program, "usePickingColor"),
                            1);
                if((move_right == 0) and (mess==0) and controllerX<sizeOfgame-2 and !(forceFall == 1)){
                    mess++;
                    controllerX++;
                    move = true;
                    go_to = 1;
                    
                } else if ((move_right == 0) and (oneTimeToRight == 1) and (mess==0) and controllerX==sizeOfgame-2 and !(forceFall == 1) and (blockToDraw == 1)){
                        cubeMatrix[0][1] = cubeMatrix[0][0];
                        cubeMatrix[0][0] = -1;
                        mess++;
                        oneTimeToRight = 0;
                        move = true;
                        go_to = 1;
                }
                
                else if ((move_right == 0) and (twoTimeToRight == 1) and (mess==0) and controllerX==sizeOfgame-2 and !(forceFall == 1) and (blockToDraw == 2 && problem == 1)){
                    if (blockToDraw == 2 && problem == 1){
                        cubeMatrix[0][1] = cubeMatrix[0][0];
                        cubeMatrix[0][0] = -1;
                        cubeMatrix[1][1] = cubeMatrix[1][0];
                        cubeMatrix[1][0] = -1;
                        mess++;
                        twoTimeToRight = 0;
                        move = true;
                        go_to = 1;
                    }
                }
                break;
                
            case GLFW_KEY_LEFT: // move the block one unit to the left
                if((move_right == 0) and (mess==0) and controllerX>0 and !(forceFall == 1)){
                    mess++;
                    controllerX--;
                    move = true;
                    go_to = 2;
                    if (blockToDraw == 1 && oneTimeToRight == 0 && controllerX==sizeOfgame-2){
                        oneTimeToRight = 1;
                        cubeMatrix[0][0] = cubeMatrix[0][1];
                        cubeMatrix[0][1] = -1;
                    }
                    if (blockToDraw == 2 && twoTimeToRight == 0 && controllerX==sizeOfgame-2){
                        twoTimeToRight = 1;
                        cubeMatrix[0][0] = cubeMatrix[0][1];
                        cubeMatrix[0][1] = -1;
                        cubeMatrix[1][0] = cubeMatrix[1][1];
                        cubeMatrix[1][1] = -1;
                    }
                }
                break;
            
            case GLFW_KEY_I: //reset the game
                init();
                problem=0;
                gameEnded = 0;
                new_obj=true;
                controllerX=initialX;
                controllerY=sizeOfgame;
                forceFall = 0;
                createNew = 1;
                break;
            case GLFW_KEY_H:
                std::cout << "Input Controls:" << std::endl;
                std::cout << "E -- End the Game" << std::endl;
                std::cout << "<- -- Move the block 1 unit to the left" << std::endl;
                std::cout << "-> -- Move the block 1 unit to the right" << std::endl;
                std::cout << "SPACE -- Rotate the Block" << std::endl;
                std::cout << "I -- Reset the Game"<< std::endl;
                std::cout << "DOWN -- Make the object fall"<< std::endl;
                std::cout << "Q -- quit (exit) the program" << std::endl;
                break;
                
                // Shading options
            case GLFW_KEY_S:
                shadingOption += 1;
                shadingOption = shadingOption % 3;
                glUniform1i(ShadingOption, shadingOption);
                break;
                
                //Light Source Position
            case GLFW_KEY_L:
                lightSourcePosition += 1;
                lightSourcePosition = lightSourcePosition % 2;
                glUniform1i(LightSourcePosition, lightSourcePosition);
                break;
                
            case GLFW_KEY_E:
                gameEnded = 1;
                break;
                
                //Material Options
            case GLFW_KEY_M:
                if(material_shininess == Plastic) {
                    material_shininess = Metallic;
                }
                else if(material_shininess == Metallic) {
                    material_shininess = Thirty;
                }
                else if(material_shininess == Thirty) {
                    material_shininess = Hundred;
                }
                else {
                    material_shininess = Plastic;
                }
                glUniform1f( glGetUniformLocation(program, "Shininess"), material_shininess );
                break;
                
                //Component options
            case GLFW_KEY_O:
                componentOption += 1;
                componentOption = componentOption % 4;
                
                if(componentOption == 0) {
                    specular_product = vec4(0, 0, 0, 0);
                }
                
                else if(componentOption == 1) {
                    diffuse_product = vec4(0, 0, 0, 0);
                    
                }
                else if (componentOption == 2) {
                    ambient_product = vec4(0, 0, 0, 0);
                }
                
                else {
                    
                    ambient_product = light_ambient * material_ambient;
                    diffuse_product = light_diffuse * material_diffuse;
                    specular_product = light_specular * material_specular;
                }
                glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
                             1, specular_product );
                glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
                             1, diffuse_product );
                glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
                             1, ambient_product );
                
                break;
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if ( action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        glDrawBuffer(GL_BACK); //back buffer is default thus no need
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if(!gameEnded){ //to assign different color to each of the cube that makes a block
            for (int i = 0; i<2 ; i++){
                for (int j = 0; j<2 ; j++){
                    pickingOption = (i*2)+j;
                    glUniform1i(UsePicking, pickingOption);
                    if(cubeMatrix[i][j] != -1) {
                        glBindVertexArray( vaoArray[cubeMatrix[i][j]] );
                        glUniformMatrix4fv( ModelView, 1, GL_TRUE, Scale(1.5, 1.5, 1.5)*modelViews[cubeMatrix[i][j]]);
                        glDrawArrays( GL_TRIANGLES, 0, NumVertices );
                    }
                }
            }
        }
        
        if (gameEnded){ // to color in the back buffer the PLAY AGAIN button
            for(int i = totalCubeRequired+scoreCube; i < totalCubeRequired+scoreCube+1; i++){
                glUniform1i(UsePicking, 0);
                glBindVertexArray( vaoArray[i] );
                glUniformMatrix4fv( ModelView, 1, GL_TRUE, Translate(0.37,1.73,1) * Scale(0.68, 0.68, 0.68)* RotateX( 0 ) * RotateY( 0 ) * RotateZ( 0.0 ));
                glDrawArrays( GL_TRIANGLES, 0, NumVertices );
            }
        }
        
        
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        
        int fb_width, fb_height;
        glfwGetFramebufferSize(window, &fb_width, &fb_height);
        
        int win_width, win_height;
        glfwGetWindowSize(window, &win_width, &win_height);
        
        //Have to differentiate between window and frame buffer sizes
        x*=(fb_width/win_width);
        y*=(fb_height/win_height);
        
        y = fb_height - y;
        
        //glReadPixels reads from frame buffer, hence use frame buffer size
        unsigned char pixel[4];
        glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
        if(!gameEnded){
            if (pixel[0]==255 && pixel[1]==0 && pixel[2]==0 && blocktoErase>0) {
                cubeMatrix[0][0] = -1;
                blocktoErase--;
            }
            else if (pixel[0]==0 && pixel[1]==255 && pixel[2]==0 && blocktoErase>0) {
                cubeMatrix[0][1] = -1;
                blocktoErase--;
            }
            else if (pixel[0]==0 && pixel[1]==0 && pixel[2]==255 && blocktoErase>0) {
                cubeMatrix[1][0] = -1;
                blocktoErase--;
            }
            else if (pixel[0]==255 && pixel[1]==255 && pixel[2]==0 && blocktoErase>0) {
                cubeMatrix[1][1] = -1;
                blocktoErase--;
            }
        }
        if(gameEnded){
            if (pixel[0]==255 && pixel[1]==0 && pixel[2]==0 ) {
                printf("reset game");
                key_callback(window, GLFW_KEY_I, 0, GLFW_RELEASE, 0);
            }
        }
       
        else std::cout << "None"<<std::endl;
        
        std::cout << "R: " << (int)pixel[0] << std::endl;
        std::cout << "G: " << (int)pixel[1] << std::endl;
        std::cout << "B: " << (int)pixel[2] << std::endl;
        std::cout << std::endl;
        pickingOption = 4;
        glUniform1i(UsePicking, pickingOption);
       // glfwSwapBuffers(window); //you can enable (and disable the other) this to display the triangles with their hidden id colors
    }
}


void
update( void )
{
    Theta[0] = Theta[1] = Theta[2] = 0.0;
    Theta[Axis] = counterCLock;

}
int
main()
{
    if (!glfwInit())
            exit(EXIT_FAILURE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    window = glfwCreateWindow(720, 720, "Spin Cube", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
   
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    init();

    double frameRate = 120
    , currentTime, previousTime = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        currentTime = glfwGetTime();
        if (currentTime - previousTime >= 1/frameRate){
            previousTime = currentTime;
            update();
        }
        
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
}
