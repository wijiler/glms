number w = 1920;
number h = 1080;

image img = image.make(w, h);

img.shade((vec3 uv, vec3 fragCoord, vec3 resolution) => {
    //  vec3 center = resolution * 0.5;
    //number d = abs(distance(fragCoord, center));
    //number g = 255 * (d < TAU * 6.0);
  return vec4(255, 0, 0, 255.0);
});

img.save("test.png");
