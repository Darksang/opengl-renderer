#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D TextureDiffuse;

void main() {
    FragColor = texture(TextureDiffuse, TexCoords);
}