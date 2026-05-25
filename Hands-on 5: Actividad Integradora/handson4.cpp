#include "raylib.h"
#include <cmath>

int main()
{
    InitWindow(1300, 800, "Practica 5 - Movimientos 3D");

    Camera3D camera = { 0 };
    camera.position = { 0.0f, 18.0f, 20.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    // =========================
    // VARIABLES DEL REBOTE
    // =========================

    float reboteY = 4.0f;
    float velocidadY = 0.0f;
    float gravedad = -0.015f;

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        float t = (float)GetTime();

        // =========================================================
        // 1. MOVIMIENTO SENOIDAL
        // y(t) = A sin(wt) + C
        // =========================================================

        float senoY = 2.0f + sinf(t * 2.0f) * 1.5f;

        // =========================================================
        // 2. REBOTE CON GRAVEDAD
        // y = y0 + v0t - 1/2 gt²
        // =========================================================

        velocidadY += gravedad;
        reboteY += velocidadY;

        // Rebote contra el piso
        if (reboteY < 1.0f)
        {
            reboteY = 1.0f;
            velocidadY *= -0.90f;
        }

        // =========================================================
        // 3. ORBITA CIRCULAR
        // x = r cos(t)
        // z = r sin(t)
        // =========================================================

        float radio = 2.0f;

        float orbitaX = cosf(t) * radio;
        float orbitaZ = sinf(t) * radio;

        // =========================================================
        // 4. TRAYECTORIA ONDULADA
        // x = vt
        // z = A sin(wt)
        // =========================================================

        float trayectoriaX = -2.5f + fmodf(t * 1.5f, 5.0f);
        float trayectoriaZ = sinf(t * 3.0f) * 1.2f;

        // =========================================================
        // DIBUJO
        // =========================================================

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawGrid(40, 1.0f);

        // =========================================================
        // DIVISIONES VISUALES
        // =========================================================

        DrawCube({ -9.0f, 0.05f, 4.0f }, 5.0f, 0.1f, 5.0f, LIGHTGRAY);
        DrawCube({ -3.0f, 0.05f, 4.0f }, 5.0f, 0.1f, 5.0f, LIGHTGRAY);
        DrawCube({  3.0f, 0.05f, 4.0f }, 5.0f, 0.1f, 5.0f, LIGHTGRAY);
        DrawCube({  9.0f, 0.05f, 4.0f }, 5.0f, 0.1f, 5.0f, LIGHTGRAY);

        DrawCubeWires({ -9.0f, 2.0f, 4.0f }, 5.0f, 4.0f, 5.0f, LIGHTGRAY);
        DrawCubeWires({ -3.0f, 2.0f, 4.0f }, 5.0f, 4.0f, 5.0f, LIGHTGRAY);
        DrawCubeWires({  3.0f, 2.0f, 4.0f }, 5.0f, 4.0f, 5.0f, LIGHTGRAY);
        DrawCubeWires({  9.0f, 2.0f, 4.0f }, 5.0f, 4.0f, 5.0f, LIGHTGRAY);

        // =========================================================
        // 1. MOVIMIENTO SENOIDAL
        // =========================================================

        // Línea guía
        DrawLine3D(
            { -9.0f, 0.5f, 4.0f },
            { -9.0f, 4.5f, 4.0f },
            BLUE
        );

        // Anillo energético
        DrawSphereWires(
            { -9.0f, senoY, 4.0f },
            0.8f,
            12,
            12,
            SKYBLUE
        );

        // Cápsula flotante
        DrawCylinder(
            { -9.0f, senoY, 4.0f },
            0.30f,
            0.30f,
            1.5f,
            16,
            BLUE
        );

        // =========================================================
        // 2. REBOTE FISICO
        // =========================================================

        // Sombra
        DrawCylinder(
            { -3.0f, 0.02f, 4.0f },
            0.55f,
            0.55f,
            0.02f,
            20,
            DARKGRAY
        );

        // Piso
        DrawCube(
            { -3.0f, 0.15f, 4.0f },
            2.0f,
            0.3f,
            2.0f,
            GRAY
        );

        // Deformación visual
        float escalaY = (reboteY <= 1.05f) ? 0.7f : 1.0f;

        DrawSphereEx(
            { -3.0f, reboteY, 4.0f },
            0.5f,
            24,
            24,
            RED
        );

        // =========================================================
        // 3. ORBITA PLANETARIA
        // =========================================================

        // Planeta central
        DrawSphere(
            { 3.0f, 1.0f, 4.0f },
            0.8f,
            DARKBLUE
        );

        // Anillo orbital
        DrawSphereWires(
            { 3.0f, 1.0f, 4.0f },
            radio,
            24,
            24,
            DARKGREEN
        );

        // Línea al satélite
        DrawLine3D(
            { 3.0f, 1.0f, 4.0f },
            { 3.0f + orbitaX, 1.0f, 4.0f + orbitaZ },
            LIME
        );

        // Satélite orbitando
        DrawCube(
            { 3.0f + orbitaX, 1.0f, 4.0f + orbitaZ },
            0.45f,
            0.45f,
            0.45f,
            GREEN
        );

        // =========================================================
        // 4. TRAYECTORIA ONDULADA
        // =========================================================

        // Camino ondulado
        for (int i = 0; i < 100; i++)
        {
            float x1 = -2.5f + i * (5.0f / 100.0f);
            float x2 = -2.5f + (i + 1) * (5.0f / 100.0f);

            float z1 = sinf(x1 * 3.0f) * 1.2f;
            float z2 = sinf(x2 * 3.0f) * 1.2f;

            DrawLine3D(
                { 9.0f + x1, 1.0f, 4.0f + z1 },
                { 9.0f + x2, 1.0f, 4.0f + z2 },
                ORANGE
            );
        }

        // Estela del dron
        for (int i = 0; i < 12; i++)
        {
            DrawSphere(
                {
                    9.0f + trayectoriaX - i * 0.08f,
                    1.0f,
                    4.0f + trayectoriaZ
                },
                0.04f,
                GOLD
            );
        }

        // Drone
        DrawCube(
            { 9.0f + trayectoriaX, 1.0f, 4.0f + trayectoriaZ },
            0.6f,
            0.25f,
            0.8f,
            ORANGE
        );

        EndMode3D();

        // =========================================================
        // TEXTO
        // =========================================================

        DrawText(
            "Practica 5 - Movimientos Basicos en 3D",
            20,
            20,
            30,
            DARKGRAY
        );

        DrawText("1. Movimiento senoidal", 90, 90, 22, BLUE);
        DrawText("y(t)=A sin(wt)+C", 110, 120, 18, DARKGRAY);

        DrawText("2. Rebote con gravedad", 360, 90, 22, RED);
        DrawText("Fisica con aceleracion", 390, 120, 18, DARKGRAY);

        DrawText("3. Orbita circular", 670, 90, 22, GREEN);
        DrawText("x=r cos(t), z=r sin(t)", 670, 120, 18, DARKGRAY);

        DrawText("4. Trayectoria ondulada", 930, 90, 22, ORANGE);
        DrawText("Movimiento parametrico", 970, 120, 18, DARKGRAY);

        DrawText(
            "Cada zona representa un modelo matematico distinto de movimiento.",
            20,
            730,
            20,
            GRAY
        );

        DrawText("ESC para salir", 20, 760, 20, GRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}