#include "Close2GL.h"

vector<vec3> Close2GL::transformPositions(vector<vec3> positions, mat4 modelViewProjection) {
    int vertexCount = positions.size();
    vector<vec3> transformedVector(vertexCount);
    for (int i = 0; i < vertexCount; i++) {
        vec4 newPosition(positions[i], 1.0f);
        newPosition = modelViewProjection * newPosition;
        newPosition.x /= newPosition.w;
        newPosition.y /= newPosition.w;
        newPosition.z /= newPosition.w;
        transformedVector[i] = newPosition;
    }
    return transformedVector;
}

vector<unsigned int> Close2GL::viewFrustumCulling(vector<unsigned int> indices, vector<vec3> positions) { 
    int indexCount = indices.size();
    for (int i=0; i+2<indexCount; i+=3) {
        vec3 vertex1 = positions[indices[i]];
        vec3 vertex2 = positions[indices[i+1]];
        vec3 vertex3 = positions[indices[i+2]];

        if (vertex1.z < -1 || vertex1.z > 1 ||
            vertex2.z < -1 || vertex2.z > 1 ||
            vertex3.z < -1 || vertex3.z > 1 ) {

            indices.erase(indices.begin()+i, indices.begin()+i+3);
            i-=3;
            indexCount-=3;

        }
    }

    return indices;
}

vector<unsigned int> Close2GL::backfaceCulling(vector<unsigned int> indices, vector<vec3> positions, bool isCW) { 
    int indexCount = indices.size();
    for (int i=0; i+2<indexCount; i+=3) {
        vec3 vertex1 = positions[indices[i]];
        vec3 vertex2 = positions[indices[i+1]];
        vec3 vertex3 = positions[indices[i+2]];

        int pair1 = vertex1.x*vertex2.y - vertex2.x*vertex1.y;
        int pair2 = vertex2.x*vertex3.y - vertex3.x*vertex2.y;
        int pair3 = vertex3.x*vertex1.y - vertex1.x*vertex3.y;

        float a = (pair1 + pair2 + pair3)/2.0f;

        if (((a > 0) && !isCW) || ((a < 0) && isCW)) {

            indices.erase(indices.begin()+i, indices.begin()+i+3);
            i-=3;
            indexCount-=3;

        }
    }

    return indices;
}

mat4 Close2GL::projectionMatrix(float FOVx, float FOVy, float nearPlane, float farPlane) {

    float nearPlaneWidth = 2 * tan(FOVx/2) * (nearPlane);
    float nearPlaneHeight = 2 * tan(FOVy/2) * (nearPlane);
    float frustumLength = farPlane - nearPlane;

    mat4 projection = mat4(0.0);

    projection[0][0] = 2 * nearPlane / nearPlaneWidth;
    projection[1][1] = 2 * nearPlane / nearPlaneHeight;
    projection[2][2] = -(farPlane + nearPlane)/ frustumLength;
    projection[3][2] = - 2 * farPlane * nearPlane / frustumLength;
    projection[2][3] = -1;

    return projection;
}

mat4 Close2GL::viewMatrix(vec3 cameraPosition, vec3 cameraDirection, vec3 cameraUp) {
    vec3 n = normalize(-cameraDirection);
    vec3 u = cross(normalize(cameraUp), n);
    vec3 v = cross(n,u);
    mat4 projection(0.0);

    projection[0][0] = u.x;
    projection[1][0] = u.y;
    projection[2][0] = u.z;
    projection[3][0] = dot(-cameraPosition, u);

    projection[0][1] = v.x;
    projection[1][1] = v.y;
    projection[2][1] = v.z;
    projection[3][1] = dot(-cameraPosition, v);

    projection[0][2] = n.x;
    projection[1][2] = n.y;
    projection[2][2] = n.z;
    projection[3][2] = dot(-cameraPosition, n);

    projection[3][3] = 1;

    return projection;
}

mat4 Close2GL::viewportMatrix(int left, int top, int right, int bottom) {
    mat4 viewport(0.0);
    
    // Scaling
    viewport[0][0] = (right - left)/2.0f;
    viewport[1][1] = (top - bottom)/2.0f;   // negative value, flips mapping
    viewport[2][2] = 1;
    viewport[3][3] = 1;

// Translation
    viewport[3][0] = (right + left)/2.0f;
    viewport[3][1] = (top + bottom)/2.0f;
    
    return viewport;
}

float Close2GL::horizontalFieldOfView(float FOVy, float screenWidth, float screenHeight) {
    return 2.0f * atan(tan(FOVy/2.0f)/screenHeight*screenWidth);
}

unsigned char* createColorBuffer(int width, int height) {
   unsigned char* buffer = new unsigned char[width * height * 3]; 
   return buffer;
}

void clearColorBuffer(unsigned char* buffer, vec3 color) {
    
}
