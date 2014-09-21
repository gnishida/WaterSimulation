uniform vec2 center[3];
uniform float frequency[3];
uniform float amplitude[3];
uniform float t;
uniform sampler2D texUnit;

void main()
{
	const float offset = 1.0 / 512.0;
	
	vec2 texCoord = gl_TexCoord[0].xy;

	// parameters for Gray-Scott model
	float F = 0.037;
	float K = 0.06;
	float Du = 0.209;
	float Dv = 0.102;

	// get the colors of the pixels
	vec2 c = texture2D(texUnit, texCoord).rb;
	vec2 c1 = texture2D(texUnit, texCoord + vec2(-offset, 0.0)).rb;
	vec2 c2 = texture2D(texUnit, texCoord + vec2(0.0, offset)).rb;
	vec2 c3 = texture2D(texUnit, texCoord + vec2(offset, 0.0)).rb;
	vec2 c4 = texture2D(texUnit, texCoord + vec2(0.0, -offset)).rb;

	float U = c.x;
	float V = c.y;
	vec2 lap = c1 + c2 + c3 + c4  - c * 4.0;

	float dU = Du * lap.x - U * V * V + F * (1.0 - U);
	float dV = Dv * lap.y + U * V * V - (F + K) * V;

	//gl_FragColor = vec4(U + dU, 0.0, V + dV, 1.0);
	float height = 0.0;
	for (int i = 0; i < 3; ++i) {
		float r = sqrt((texCoord.x - center[i].x) * (texCoord.x - center[i].x) + (texCoord.y - center[i].y) * (texCoord.y - center[i].y));
		height += amplitude[i] * sin(r * frequency[i] - t);
	}
	gl_FragColor = vec4((height + 1.0) * 0.5, (height + 1.0) * 0.5, 1.0, 1.0);
}