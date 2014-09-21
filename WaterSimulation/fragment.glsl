uniform vec2 center[3];
uniform float frequency[3];
uniform float amplitude[3];
uniform vec3 lightDir;
uniform float t;
uniform sampler2D texUnit;

void main()
{
	vec2 texCoord = gl_TexCoord[0].xy;

	float height = 0.0;
	vec3 n = vec3(0, 0, 1);
	for (int i = 0; i < 3; ++i) {
		float dx = abs(texCoord.x - center[i].x);
		float dy = abs(texCoord.y - center[i].y);
		float dr = sqrt(dx * dx + dy * dy);
		height += amplitude[i] * sin(dr * frequency[i] - t);

		if (dr > 0.0) {
			n.x += cos(dr * frequency[i] - t) * dx / dr;
			n.y += cos(dr * frequency[i] - t) * dy / dr;
		}
	}
	n = normalize(n);

	vec3 l = normalize(lightDir);

	float diffuse = max(0.0, dot(n, l));
	//gl_FragColor = vec4(l, 1.0);
	gl_FragColor = vec4(diffuse, diffuse, 1.0, 1.0);
}