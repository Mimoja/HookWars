#version 330 core

layout(location = 0) out float FragDepth;

void main(){
	FragDepth = gl_FragCoord.z;
}