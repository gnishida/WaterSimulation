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
		float dx = texCoord.x - center[i].x;
		float dy = texCoord.y - center[i].y;
		float dr = sqrt(dx * dx + dy * dy);
		height += amplitude[i] * sin(dr * frequency[i] - t);

		if (dr > 0.0) {
			n.x -= amplitude[i] * frequency[i] * cos(dr * frequency[i] - t) * dx / dr * 0.033;
			n.y -= amplitude[i] * frequency[i] * cos(dr * frequency[i] - t) * dy / dr * 0.033;
		}
	}
	n = normalize(n);

	vec3 l = normalize(lightDir);

	float diffuse = max(0.0, dot(n, l));
	gl_FragColor = vec4(diffuse * 0.9 + 0.1, diffuse * 0.9 + 0.1, 1.0, 1.0);
}