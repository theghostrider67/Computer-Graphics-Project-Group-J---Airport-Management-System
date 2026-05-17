#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib> // For random numbers
#include <ctime>   // For random seed

// --- Global State ---
int currentAirport = 1;
int flightStage = 0;

float planeX = -1.2f;
float planeY = -0.7f;
float planeAngle = 0.0f;
float cloudX = -1.5f;

// --- Speed & Control Flags ---
float baseSpeed = 0.012f;
float planeSpeed = 0.012f;
bool isStopped = false;

// --- Environmental Toggles ---
bool isNight = false;
bool isRaining = false;
bool isSnowing = false;

// --- Sliding Text & Window State ---
float signScroll = 0.45f;
int winW = 1200;
int winH = 650;

// --- Weather & Environment Particles ---
#define MAX_PARTICLES 300
struct Particle { float x, y, speed, drift; };
Particle weather[MAX_PARTICLES];
struct Star { float x, y; };
Star stars[100];

// Initializes random positions for stars and weather particles
void initEnvironment() {
    srand((unsigned int)time(0));
    // Initialize Weather Particles
    for (int i = 0; i < MAX_PARTICLES; i++) {
        weather[i].x = ((rand() % 500) / 100.0f) - 2.5f; // -2.5 to 2.5
        weather[i].y = ((rand() % 200) / 100.0f) - 1.0f; // -1.0 to 1.0
        weather[i].speed = ((rand() % 100) / 2000.0f) + 0.02f;
        weather[i].drift = ((rand() % 100) / 10000.0f);
    }
    // Initialize Stars
    for (int i = 0; i < 100; i++) {
        stars[i].x = ((rand() % 500) / 100.0f) - 2.5f;
        stars[i].y = ((rand() % 100) / 100.0f); // 0.0 to 1.0 (Top half)
    }
}

// --- Math Utilities ---
void drawCircle(float x, float y, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * 3.1415926f * i / 100;
        glVertex2f(x + radius * cosf(angle), y + radius * sinf(angle));
    }
    glEnd();
}

void drawCloud(float x, float y, float scale, float r = 1.0f, float g = 1.0f, float b = 1.0f) {
    glColor3f(r, g, b);
    drawCircle(x, y, 0.1f * scale);
    drawCircle(x + 0.08f * scale, y + 0.03f * scale, 0.12f * scale);
    drawCircle(x + 0.15f * scale, y, 0.1f * scale);
}

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *text++);
    }
}

// --- Visual Elements ---
void drawSignboard(float centerX, float centerY, const char* text) {
    float boxWidth = 0.9f;
    float height = 0.12f;

    glColor3f(0.1f, 0.1f, 0.1f);
    glRectf(centerX - boxWidth/2.0f, centerY - height/2.0f, centerX + boxWidth/2.0f, centerY + height/2.0f);

    glColor3f(0.7f, 0.7f, 0.75f);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(centerX - boxWidth/2.0f, centerY - height/2.0f);
    glVertex2f(centerX + boxWidth/2.0f, centerY - height/2.0f);
    glVertex2f(centerX + boxWidth/2.0f, centerY + height/2.0f);
    glVertex2f(centerX - boxWidth/2.0f, centerY + height/2.0f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_SCISSOR_TEST);
    int pixX = (int)(((centerX - boxWidth/2.0f) + 1.5f) / 3.0f * winW);
    int pixY = (int)(((centerY - height/2.0f) + 1.0f) / 2.0f * winH);
    int pixW = (int)(boxWidth / 3.0f * winW);
    int pixH = (int)(height / 2.0f * winH);
    glScissor(pixX, pixY, pixW, pixH);
    float startX = centerX + signScroll;
    drawText(startX, centerY - 0.02f, text);
    glDisable(GL_SCISSOR_TEST);
}

void drawBDFlag(float poleX, float poleY, float scale) {
    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(3); glBegin(GL_LINES); glVertex2f(poleX, poleY); glVertex2f(poleX, poleY + 0.2f * scale); glEnd(); glLineWidth(1);
    glColor3f(0.0f, 0.42f, 0.24f); glRectf(poleX, poleY + 0.08f * scale, poleX + 0.2f * scale, poleY + 0.2f * scale);
    glColor3f(0.86f, 0.16f, 0.24f); drawCircle(poleX + 0.09f * scale, poleY + 0.14f * scale, 0.04f * scale);
}

void drawModernTerminal(float r, float g, float b, float glassR, float glassG, float glassB) {
    glColor3f(r, g, b); glRectf(-1.4f, -0.7f, 0.1f, 0.0f);
    float r2 = r + 0.1f; if(r2 > 1.0f) r2 = 1.0f;
    float g2 = g + 0.1f; if(g2 > 1.0f) g2 = 1.0f;
    float b2 = b + 0.1f; if(b2 > 1.0f) b2 = 1.0f;
    glColor3f(r2, g2, b2); glRectf(-1.2f, 0.0f, -0.2f, 0.2f);
    glColor3f(glassR, glassG, glassB);
    for(float wx = -1.35f; wx < 0.0f; wx += 0.2f) glRectf(wx, -0.5f, wx + 0.15f, -0.2f);
    for(float wx = -1.15f; wx < -0.3f; wx += 0.2f) glRectf(wx, 0.05f, wx + 0.15f, 0.15f);
    glColor3f(0.1f, 0.1f, 0.1f); glRectf(-1.1f, -0.7f, -1.0f, -0.55f); glRectf(-0.3f, -0.7f, -0.2f, -0.55f);
    glColor3f(0.5f, 0.5f, 0.5f); glRectf(0.1f, -0.6f, 0.3f, -0.5f); glRectf(0.3f, -0.65f, 0.4f, -0.45f);
}

void drawGroundAndRunway() {
    if (currentAirport == 1) glColor3f(0.3f, 0.65f, 0.3f);
    else if (currentAirport == 2) glColor3f(0.2f, 0.45f, 0.2f);
    else glColor3f(0.25f, 0.5f, 0.25f);

    glRectf(-2.5f, -1.0f, 2.5f, -0.3f);

    glColor3f(0.18f, 0.18f, 0.18f);
    glRectf(-2.5f, -0.92f, 2.5f, -0.65f);

    glColor3f(1.0f, 1.0f, 1.0f);
    for (float i = -2.4f; i < 2.5f; i += 0.6f) glRectf(i, -0.8f, i + 0.3f, -0.77f);
}

// --- AIRPORT DRAWINGS ---
void drawRafiAirport() {
    if (isNight) glClearColor(0.05f, 0.1f, 0.15f, 1.0f);
    else glClearColor(0.35f, 0.65f, 0.95f, 1.0f);

    glColor3f(0.4f, 0.5f, 0.6f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-1.5f, -0.3f); glVertex2f(-0.8f, 0.2f); glVertex2f(-0.1f, -0.3f);
    glVertex2f(0.5f, -0.3f); glVertex2f(1.2f, 0.3f); glVertex2f(1.9f, -0.3f);
    glEnd();

    drawGroundAndRunway();
    drawModernTerminal(0.8f, 0.82f, 0.85f, 0.3f, 0.6f, 0.8f);

    // ==========================================
    // NEW AESTHETIC TOWER FOR RAFI'S AIRPORT
    // ==========================================
    float tx = 0.6f, ty = 0.25f;

    // 1. Sleek Base Pedestal
    glColor3f(0.6f, 0.62f, 0.65f);
    glBegin(GL_POLYGON);
    glVertex2f(tx - 0.05f, -0.7f);
    glVertex2f(tx + 0.05f, -0.7f);
    glVertex2f(tx + 0.02f, ty);
    glVertex2f(tx - 0.02f, ty);
    glEnd();

    // 2. The Glass Core (Glowing Cyan Pearl)
    glColor3f(0.1f, 0.7f, 0.9f); // Bright cyan
    drawCircle(tx, ty + 0.1f, 0.16f);
    glColor3f(0.6f, 0.9f, 1.0f); // Glass reflection highlight
    drawCircle(tx - 0.04f, ty + 0.14f, 0.05f);

    // 3. Left Sweeping Architectural Sail
    glColor3f(0.9f, 0.92f, 0.95f); // Bright sleek white
    glBegin(GL_POLYGON);
    glVertex2f(tx - 0.05f, -0.7f);
    glVertex2f(tx - 0.25f, ty + 0.1f);
    glVertex2f(tx, ty + 0.35f);
    glVertex2f(tx - 0.12f, ty + 0.1f);
    glEnd();

    // 4. Right Sweeping Architectural Sail
    glColor3f(0.75f, 0.78f, 0.82f); // Slightly shaded silver
    glBegin(GL_POLYGON);
    glVertex2f(tx + 0.05f, -0.7f);
    glVertex2f(tx + 0.25f, ty + 0.1f);
    glVertex2f(tx, ty + 0.35f);
    glVertex2f(tx + 0.12f, ty + 0.1f);
    glEnd();

    // 5. Floating Observation Deck (Halo Ring)
    glColor3f(0.2f, 0.25f, 0.3f);
    glRectf(tx - 0.28f, ty + 0.08f, tx + 0.28f, ty + 0.12f);
    glColor3f(0.0f, 1.0f, 0.8f); // Neon green/blue strip on the deck
    glRectf(tx - 0.28f, ty + 0.1f, tx + 0.28f, ty + 0.11f);

    // 6. Central Spire & Beacon
    glColor3f(0.5f, 0.5f, 0.55f);
    glRectf(tx - 0.01f, ty + 0.35f, tx + 0.01f, ty + 0.6f);
    glColor3f(1.0f, 0.2f, 0.2f);
    drawCircle(tx, ty + 0.6f, 0.015f); // Red beacon

    // 7. Flag and Signboard
    drawBDFlag(tx, ty + 0.45f, 1.2f);
    drawSignboard(tx, -0.1f, "Rafi's International Airport");
    // ==========================================

    if (!isNight) drawCloud(cloudX, 0.7f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void drawTasinAirport() {
    if (isNight) glClearColor(0.06f, 0.08f, 0.12f, 1.0f);
    else glClearColor(0.45f, 0.7f, 0.85f, 1.0f);

    glColor3f(0.3f, 0.3f, 0.35f);
    for(float i = -1.8f; i < 2.0f; i += 0.4f) glRectf(i, -0.3f, i+0.1f, -0.1f);

    drawGroundAndRunway();
    glColor3f(0.5f, 0.5f, 0.55f); glRectf(-2.2f, -0.7f, -1.8f, -0.1f); glRectf(-1.7f, -0.7f, -1.3f, -0.1f);
    glColor3f(0.7f, 0.2f, 0.2f); drawCircle(-2.0f, -0.1f, 0.2f); drawCircle(-1.5f, -0.1f, 0.2f);

    drawModernTerminal(0.4f, 0.4f, 0.45f, 0.8f, 0.7f, 0.2f);

    float tx = 0.8f;
    glColor3f(0.35f, 0.35f, 0.35f); glRectf(tx - 0.15f, -0.7f, tx + 0.15f, 0.3f);
    glColor3f(0.2f, 0.2f, 0.25f); glRectf(tx - 0.2f, 0.3f, tx + 0.2f, 0.55f);
    glColor3f(0.8f, 0.7f, 0.2f); glRectf(tx - 0.15f, 0.35f, tx + 0.15f, 0.5f);

    glColor3f(0.2f, 0.2f, 0.2f); glBegin(GL_LINES); glVertex2f(tx - 0.1f, 0.55f); glVertex2f(tx - 0.1f, 0.7f); glEnd();
    glBegin(GL_LINES); glVertex2f(tx + 0.1f, 0.55f); glVertex2f(tx + 0.1f, 0.65f); glEnd();
    drawBDFlag(tx, 0.55f, 1.2f);

    drawSignboard(tx, 0.1f, "Tasin's International Airport");
    if (!isNight) drawCloud(cloudX, 0.7f, 1.0f);
}

void drawFahimAirport() {
    if (isNight) glClearColor(0.04f, 0.05f, 0.1f, 1.0f);
    else glClearColor(0.4f, 0.65f, 0.9f, 1.0f);

    glColor3f(0.2f, 0.2f, 0.25f);
    glRectf(-1.8f, -0.3f, -1.6f, 0.0f); glRectf(-1.55f, -0.3f, -1.4f, 0.1f);
    glRectf(1.4f, -0.3f, 1.6f, 0.05f); glRectf(1.65f, -0.3f, 1.8f, 0.15f);

    drawGroundAndRunway();
    drawModernTerminal(0.7f, 0.75f, 0.8f, 0.2f, 0.4f, 0.6f);

    glColor3f(0.4f, 0.4f, 0.45f); glRectf(1.1f, -0.7f, 1.6f, -0.4f);
    glColor3f(0.6f, 0.6f, 0.6f); drawCircle(1.35f, -0.4f, 0.1f);

    float tx = 0.7f;
    glColor3f(0.85f, 0.85f, 0.85f); glRectf(tx-0.06f, -0.7f, tx+0.06f, 0.5f);
    glColor3f(0.2f, 0.2f, 0.2f); glBegin(GL_POLYGON);
    glVertex2f(tx-0.12f, 0.5f); glVertex2f(tx+0.12f, 0.5f); glVertex2f(tx+0.18f, 0.7f); glVertex2f(tx-0.18f, 0.7f); glEnd();

    drawBDFlag(tx, 0.7f, 1.2f);
    drawSignboard(tx, 0.2f, "Fahim's International Airport");
    if (!isNight) drawCloud(cloudX, 0.7f, 1.0f);
}

// Draws Stars and Night filter if Night Mode is active
void drawStars() {
    if (!isNight) return;
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 100; i++) glVertex2f(stars[i].x, stars[i].y);
    glEnd();
}

void drawNightOverlay() {
    if (!isNight) return;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.05f, 0.15f, 0.4f); // Transparent dark tint over the whole world
    glRectf(-3.0f, -3.0f, 3.0f, 3.0f);
    glDisable(GL_BLEND);
}

// Renders falling particles for Rain or Snow
void drawWeather() {
    if (!isRaining && !isSnowing) return;

    if (isRaining) {
        glColor3f(0.6f, 0.7f, 0.8f); // Greyish-blue rain
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        for (int i = 0; i < MAX_PARTICLES; i++) {
            glVertex2f(weather[i].x, weather[i].y);
            glVertex2f(weather[i].x - 0.02f, weather[i].y - 0.06f); // Slanted rain
        }
        glEnd();
        glLineWidth(1.0f);
    } else if (isSnowing) {
        glColor3f(1.0f, 1.0f, 1.0f); // White snow
        glPointSize(3.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < MAX_PARTICLES; i++) {
            glVertex2f(weather[i].x, weather[i].y);
        }
        glEnd();
    }
}

// Draws the airplane based on current position and angle
void drawPlane() {
    glPushMatrix();
    glTranslatef(planeX, planeY, 0.0f);
    glRotatef(planeAngle, 0.0f, 0.0f, 1.0f);
    glScalef(1.4f, 1.4f, 1.0f);

    glColor3f(0.95f, 0.95f, 0.95f);
    glBegin(GL_POLYGON); // Fuselage
    glVertex2f(-0.1f, -0.02f); glVertex2f(0.08f, -0.02f); glVertex2f(0.12f, 0.0f);
    glVertex2f(0.08f, 0.02f); glVertex2f(-0.1f, 0.02f);
    glEnd();

    glColor3f(1.0f, 0.8f, 0.0f); glRectf(-0.1f, -0.005f, 0.09f, 0.005f); // Stripe

    glColor3f(0.1f, 0.3f, 0.8f); // Wings
    glBegin(GL_TRIANGLES);
    glVertex2f(0.02f, 0.02f); glVertex2f(-0.04f, 0.1f); glVertex2f(-0.04f, 0.02f);
    glVertex2f(0.02f, -0.02f); glVertex2f(-0.04f, -0.1f); glVertex2f(-0.04f, -0.02f);
    glVertex2f(-0.1f, 0.02f); glVertex2f(-0.12f, 0.06f); glVertex2f(-0.06f, 0.02f);
    glEnd();

    glColor3f(0.1f, 0.1f, 0.1f); drawCircle(-0.06f, -0.025f, 0.015f); drawCircle(0.06f, -0.025f, 0.01f); // Wheels
    glPopMatrix();
}

// Key handler for toggles (S, W, Night, Weather)
void handleKeypress(unsigned char key, int x, int y) {
    if (key == 's' || key == 'S') {
        if (planeY <= -0.69f) isStopped = true;
    } else if (key == 'w' || key == 'W') {
        isStopped = false;
    } else if (key == 'n' || key == 'N') {
        isNight = true;
    } else if (key == 'd' || key == 'D') {
        isNight = false;
    } else if (key == 'r' || key == 'R') {
        isRaining = !isRaining;
        isSnowing = false; // Turn off snow if raining
    } else if (key == 't' || key == 'T') {
        isSnowing = !isSnowing;
        isRaining = false; // Turn off rain if snowing
    }
    glutPostRedisplay();
}

// Arrow key handler for speed
void handleSpecialKeypress(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            planeSpeed += 0.002f;
            break;
        case GLUT_KEY_DOWN:
            planeSpeed -= 0.002f;
            if (planeSpeed < 0.002f) planeSpeed = 0.002f; // Minimum speed cap
            break;
    }
    glutPostRedisplay();
}

// Main animation loop
void update(int value) {
    cloudX += 0.003f;
    if (cloudX > 2.2f) cloudX = -2.2f;

    signScroll -= 0.006f;
    if (signScroll < -1.2f) signScroll = 0.45f;

    // Updates falling weather particles
    if (isRaining || isSnowing) {
        for (int i = 0; i < MAX_PARTICLES; i++) {
            weather[i].y -= (isRaining ? weather[i].speed * 1.0f : weather[i].speed * 0.2f);
            if (isSnowing) weather[i].x += (sin(weather[i].y * 10.0f) * weather[i].drift); // Gentle snow drift

            if (weather[i].y < -1.0f) {
                weather[i].y = 1.0f;
                weather[i].x = ((rand() % 500) / 100.0f) - 2.5f;
            }
        }
    }

    // Handles the automatic multi-stage flight path
    if (!isStopped) {
        planeX += planeSpeed;
        float ratio = planeSpeed / baseSpeed; // Ensures animation curves stay smooth at any speed

        switch (flightStage) {
            case 0: currentAirport = 1;
                if (planeX > 0.1f) { planeY += 0.008f * ratio; if (planeAngle < 20.0f) planeAngle += 0.5f * ratio; }
                if (planeX > 2.0f) { flightStage = 1; planeX = -2.0f; planeY = 0.8f; planeAngle = -15.0f; }
                break;
            case 1: currentAirport = 2;
                if (planeY > -0.7f) { planeY -= 0.008f * ratio; if (planeAngle < 0.0f) planeAngle += 0.2f * ratio; }
                else { planeY = -0.7f; planeAngle = 0.0f; if (planeX > 0.5f) flightStage = 2; }
                break;
            case 2:
                if (planeX > 0.8f) { planeY += 0.01f * ratio; if (planeAngle < 20.0f) planeAngle += 0.8f * ratio; }
                if (planeX > 2.0f) { flightStage = 3; planeX = -2.0f; planeY = 0.8f; planeAngle = -15.0f; }
                break;
            case 3: currentAirport = 3;
                if (planeY > -0.7f) { planeY -= 0.008f * ratio; if (planeAngle < 0.0f) planeAngle += 0.2f * ratio; }
                else { planeY = -0.7f; planeAngle = 0.0f; }
                if (planeX > 2.0f) { flightStage = 0; planeX = -1.2f; planeY = -0.7f; planeAngle = 0.0f; }
                break;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Master rendering function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (currentAirport == 1) drawRafiAirport();
    else if (currentAirport == 2) drawTasinAirport();
    else drawFahimAirport();

    drawStars();
    drawNightOverlay();
    drawWeather();
    drawPlane();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    winW = w;
    winH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.5, 1.5, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1200, 650);
    glutCreateWindow("Triple Airport System - Aesthetic Update");

    initEnvironment();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);

    glutKeyboardFunc(handleKeypress);
    glutSpecialFunc(handleSpecialKeypress);

    glutMainLoop();
    return 0;
}
