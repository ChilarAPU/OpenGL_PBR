#version 460
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float exposure; //exposure of HDR

const float offset = 1.0 / 300.0;

void main()
{ 
    //Inverted colors
    //FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

    //Grayscale color
    //FragColor = texture(screenTexture, TexCoords);
    //float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b; //weighted channels
    //FragColor = vec4(average, average, average, 1.0);

    //3x3 kernal matrix using the specified offset
    vec2 offsets[9] = vec2[] (
        vec2(-offset, offset), //top-left
        vec2(0.0f, offset), //top center
        vec2(offset, offset), //top right
        vec2(-offset, 0.0f), //center left
        vec2(0.0f, 0.0f), //center center
        vec2(offset, 0.0f), //center right
        vec2(-offset, -offset), //bottom left
        vec2(0.0f, -offset), //bottom center
        vec2(offset, -offset) //bottom right
    );

    //specify the beginning kernal matrix (in this case being a sharpen kernal effect)
    float sharpenKernal[9] = float[] (
        -1, -1, -1,
        -1, 9, -1,
        -1, -1, -1
    );

    //blurs the colors 
    float blurKernal[9] = float[] (
        1.0/16, 2.0/16, 1.0/16,
        2.0/16, 4.0/16, 2.0/16,
        1.0/16, 2.0/16, 1.0/16
    );

    //Highlight all edges and darken the rest
    float edgeKernal[9] = float[] (
        1.0, 1.0, 1.0,
        1.0, -8.0, 1.0,
        1.0, 1.0, 1.0
    );

     //Highlight all edges and darken the rest
    float identityKernal[9] = float[] (
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 0.0
    );

    vec3 sampleTex[9];
    //get color value for the 9 positions specified in offsets[]
    for (int i = 0; i < 9; i++){
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    //Multiply current color value by all colors around it as well as the specified kernal
    for (int i = 0; i < 9; i++) {
        col += sampleTex[i] * identityKernal[i];
    }
    FragColor = vec4(col, 1.0);

    //Shadow map debugging
    //float depthValue = texture(screenTexture, TexCoords).r;
    // FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    //FragColor = vec4(vec3(depthValue), 1.0); // orthographic

    //HDR mapping to LDR
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
    //Apply gamma correction (translate final output from linear to non-linear color space)
	const float gamma = 2.2; //gamma ratio
    
    //Reinhard Tone Mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}