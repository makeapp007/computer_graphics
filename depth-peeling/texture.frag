#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture samplers

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;
uniform sampler2D ourTexture4;
uniform sampler2D ourTexture5;
uniform sampler2D ourTexture6;
uniform sampler2D ourTexture7;
uniform sampler2D ourTexture8;

uniform int directionx;
uniform int directiony;

const float offset=1.0/300;
float scale[8] = float[]( 
		0.9,1.3,1.7,2.1,2.5,2.9,3.3,3.7
	);

void main()
{
	for (int i = 0;i<8;i++){
		scale[i]*=0.5;
	}
	//////////////////////////////////////

	// Linearly interpolate between both textures (second texture is only slightly combined)
	//color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
	//color = vec4(1,0,0,1);
	// vec3 sampleTex[9];

	// for (int i = 0; i < 9; ++i)
	// {
	// 	sampleTex[i] = vec3(texture(ourTexture1,TexCoord.st+offsets[i]));
	// }
	// vec3 col;
	//for (int j = 0; j < 1; ++j){
	
		// for (int i = 0; i < 9; ++i)
		// {
		// 	//if (sampleTex[i].xyz!=vec3(0,0,0) )
		// 	col += sampleTex[i]*kernelr[i];
		// }
	

	// if (color.xyz==vec3(0,0,0) ){
	// 	color = vec4(vec3(color),0);
	// }
	//else{
	// if(direction[0]>0){
	// 	color = vec4(vec3(texture(ourTexture1,TexCoord.st+20*vec2(-1/300,0.0f)  )),1.0);
	// }
	// else{

	// 	color = vec4(vec3(texture(ourTexture1,TexCoord.st+330*vec2(10,0)  )),1.0);
	// }
	color = vec4(vec3(texture(ourTexture8,TexCoord.st)),1.0);

	vec4 color0= vec4(vec3(texture(ourTexture0,TexCoord.st+scale[0]*directionx*vec2(offset,0.0f)+scale[0]*directiony*vec2(0.0f,offset)  ) ),1.0);
	vec4 color1= vec4(vec3(texture(ourTexture1,TexCoord.st+scale[1]*directionx*vec2(offset,0.0f)+scale[1]*directiony*vec2(0.0f,offset))),1.0);
	vec4 color2= vec4(vec3(texture(ourTexture2,TexCoord.st+scale[2]*directionx*vec2(offset,0.0f)+scale[2]*directiony*vec2(0.0f,offset))),1.0);
	vec4 color3= vec4(vec3(texture(ourTexture3,TexCoord.st+scale[3]*directionx*vec2(offset,0.0f)+scale[3]*directiony*vec2(0.0f,offset))),1.0);
	vec4 color4= vec4(vec3(texture(ourTexture4,TexCoord.st+scale[4]*directionx*vec2(offset,0.0f)+scale[4]*directiony*vec2(0.0f,offset))),1.0);
	vec4 color5= vec4(vec3(texture(ourTexture5,TexCoord.st+scale[5]*directionx*vec2(offset,0.0f)+scale[5]*directiony*vec2(0.0f,offset))),1.0);
	vec4 color6= vec4(vec3(texture(ourTexture6,TexCoord.st+scale[6]*directionx*vec2(offset,0.0f)+scale[6]*directiony*vec2(0.0f,offset))),1.0);
	vec4 color7= vec4(vec3(texture(ourTexture7,TexCoord.st+scale[7]*directionx*vec2(offset,0.0f)+scale[7]*directiony*vec2(0.0f,offset))),1.0);

	// painter's algorithm
	if (color0.xyz!=vec3(0,0,0)) color = color0;
	if (color1.xyz!=vec3(0,0,0)) color = color1;
	if (color2.xyz!=vec3(0,0,0)) color = color2;
	if (color3.xyz!=vec3(0,0,0)) color = color3;
	if (color4.xyz!=vec3(0,0,0)) color = color4;
	if (color5.xyz!=vec3(0,0,0)) color = color5;
	if (color6.xyz!=vec3(0,0,0)) color = color6;
	if (color7.xyz!=vec3(0,0,0)) color = color7;
	//}
	//color += texture(ourTexture1,TexCoord);
	//color = texture(ourTexture2,TexCoord);//+texture(ourTexture1,TexCoord);
}

