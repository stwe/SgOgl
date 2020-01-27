#version 430

// terrain_quadtree_wireframe/Vertex.vert

// In

layout (location = 0) in vec2 aPosition;

// Out

out vec2 mapCoord_TC;

// Uniforms

uniform mat4 localMatrix;
uniform mat4 worldMatrix;

uniform vec3 cameraPosition;
uniform float scaleY;
uniform int lod;
uniform vec2 index;
uniform float gap;
uniform vec2 location;
uniform int lodMorphArea[8];
uniform float morphingEnabled;

uniform sampler2D heightmap;

// Function

float MorphLatitude(vec2 t_position)
{
    vec2 frac = t_position - location;

    if (index == vec2(0.0, 0.0))
    {
        float morph = frac.x - frac.y;
        if (morph > 0.0)
        {
            return morph;
        }
    }

    if (index == vec2(1.0, 0.0))
    {
        float morph = gap - frac.x - frac.y;
        if (morph > 0.0)
        {
            return morph;
        }
    }

    if (index == vec2(0.0, 1.0))
    {
        float morph = frac.x + frac.y - gap;
        if (morph > 0.0)
        {
            return -morph;
        }
    }

    if (index == vec2(1.0, 1.0))
    {
        float morph = frac.y - frac.x;
        if (morph > 0.0)
        {
            return -morph;
        }
    }

    return 0.0;
}

float MorphLongitude(vec2 t_position)
{
    vec2 frac = t_position - location;

    if (index == vec2(0.0, 0.0))
    {
        float morph = frac.y - frac.x;
        if (morph > 0.0)
        {
            return -morph;
        }
    }

    if (index == vec2(1.0, 0.0))
    {
        float morph = frac.y - (gap - frac.x);
        if (morph > 0.0)
        {
            return morph;
        }
    }

    if (index == vec2(0.0, 1.0))
    {
        float morph = gap - frac.y - frac.x;
        if (morph > 0.0)
        {
            return -morph;
        }
    }

    if (index == vec2(1.0, 1.0))
    {
        float morph = frac.x - frac.y;
        if (morph > 0.0)
        {
            return morph;
        }
    }

    return 0.0;
}

vec2 Morph(vec2 t_localPosition, int t_morphArea)
{
    vec2 morphing = vec2(0.0, 0.0);

    vec2 fixPointLatitude = vec2(0.0, 0.0);
    vec2 fixPointLongitude = vec2(0.0, 0.0);
    float distLatitude;
    float distLongitude;

    if (index == vec2(0.0, 0.0))
    {
        fixPointLatitude = location + vec2(gap, 0.0);
        fixPointLongitude = location + vec2(0.0, gap);
    }
    else if (index == vec2(1.0, 0.0))
    {
        fixPointLatitude = location;
        fixPointLongitude = location + vec2(gap, gap);
    }
    else if (index == vec2(0.0, 1.0))
    {
        fixPointLatitude = location + vec2(gap, gap);
        fixPointLongitude = location;
    }
    else if (index == vec2(1.0, 1.0))
    {
        fixPointLatitude = location + vec2(0.0, gap);
        fixPointLongitude = location + vec2(gap, 0.0);
    }
    
    float planarFactor = 0.0;
    if (cameraPosition.y > abs(scaleY))
    {
        planarFactor = 1.0;
    }
    else
    {
        planarFactor = cameraPosition.y / abs(scaleY);
    }

    distLatitude = length(cameraPosition - (worldMatrix * vec4(fixPointLatitude.x, planarFactor, fixPointLatitude.y, 1.0)).xyz);
    distLongitude = length(cameraPosition - (worldMatrix * vec4(fixPointLongitude.x, planarFactor, fixPointLongitude.y, 1.0)).xyz);

    if (distLatitude > t_morphArea)
    {
        morphing.x = MorphLatitude(t_localPosition.xy);
    }

    if (distLongitude > t_morphArea)
    {
        morphing.y = MorphLongitude(t_localPosition.xy);
    }

    return morphing;
}

// Main

void main()
{
    vec2 localPosition = (localMatrix * vec4(aPosition.x, 0.0, aPosition.y, 1.0)).xz;

    float height = texture(heightmap, localPosition).r;

    if (morphingEnabled > 0.5)
    {
        if (lod > 0)
        {
            localPosition += Morph(localPosition, lodMorphArea[lod - 1]);
            height = texture(heightmap, localPosition).r;
        }
    }

    mapCoord_TC = localPosition;

    gl_Position = worldMatrix * vec4(localPosition.x, height, localPosition.y, 1.0);
}
