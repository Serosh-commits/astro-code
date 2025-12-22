#include <raylib.h>
#include <vector>
#include <string>
#include <cmath>

const int WIDTH = 1400;
const int HEIGHT = 900;

float planck(float wavelength, float temp) {
    float h = 6.626e-34f;
    float c = 3.0e8f;
    float k = 1.381e-23f;
    float a = 2.0f * h * c * c / (wavelength * wavelength * wavelength * wavelength * wavelength);
    float b = h * c / (wavelength * k * temp);
    return a / (expf(b) - 1.0f);
}

struct SpectrumPoint {
    float wavelength;
    float intensity;
};

std::vector<SpectrumPoint> generateSpectrum(float temp, int samples = 400) {
    std::vector<SpectrumPoint> points;
    float start = 300e-9f;
    float end = 1000e-9f;
    for (int i = 0; i < samples; ++i) {
        float wl = start + (end - start) * i / (samples - 1);
        float intensity = planck(wl, temp);
        points.push_back({wl * 1e9f, intensity});
    }
    return points;
}

Color wavelengthToColor(float wl_nm) {
    if (wl_nm < 380) return PURPLE;
    if (wl_nm < 420) return VIOLET;
    if (wl_nm < 490) return BLUE;
    if (wl_nm < 570) return GREEN;
    if (wl_nm < 590) return YELLOW;
    if (wl_nm < 620) return ORANGE;
    return RED;
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Blackbody Spectrum Visualizer");
    SetTargetFPS(60);

    float temperature = 5772.0f;
    bool showGraph = true;

    std::vector<SpectrumPoint> currentSpectrum = generateSpectrum(temperature);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_UP)) temperature += 50;
        if (IsKeyDown(KEY_DOWN) && temperature > 1000) temperature -= 50;
        if (IsKeyPressed(KEY_SPACE)) showGraph = !showGraph;
        if (IsKeyPressed(KEY_R)) currentSpectrum = generateSpectrum(temperature);

        currentSpectrum = generateSpectrum(temperature);

        BeginDrawing();
        ClearBackground(BLACK);

        float maxIntensity = 0;
        for (const auto& p : currentSpectrum) {
            if (p.intensity > maxIntensity) maxIntensity = p.intensity;
        }

        if (showGraph) {
            int graphX = 100;
            int graphY = 150;
            int graphW = WIDTH - 200;
            int graphH = HEIGHT - 300;

            DrawRectangleLines(graphX, graphY, graphW, graphH, WHITE);
            DrawText("Wavelength (nm)", WIDTH / 2 - 100, HEIGHT - 100, 20, WHITE);
            DrawText("Intensity", 20, HEIGHT / 2 - 50, 20, WHITE);

            for (size_t i = 1; i < currentSpectrum.size(); ++i) {
                float x1 = graphX + (currentSpectrum[i-1].wavelength - 300) / 700.0f * graphW;
                float x2 = graphX + (currentSpectrum[i].wavelength - 300) / 700.0f * graphW;
                float y1 = graphY + graphH - (currentSpectrum[i-1].intensity / maxIntensity) * graphH;
                float y2 = graphY + graphH - (currentSpectrum[i].intensity / maxIntensity) * graphH;

                Color col = wavelengthToColor(currentSpectrum[i].wavelength);
                DrawLineEx({x1, y1}, {x2, y2}, 3.0f, col);
            }

            float peakWL = 2.897e6f / temperature;
            float peakX = graphX + (peakWL - 300) / 700.0f * graphW;
            DrawLineEx({peakX, graphY}, {peakX, graphY + graphH}, 2.0f, YELLOW);
        }

        DrawCircle(WIDTH / 2, 100, 60, BLACK);
        Color starColor = wavelengthToColor(2.897e6f / temperature);
        DrawCircle(WIDTH / 2, 100, 58, starColor);

        DrawText(TextFormat("Temperature: %.0f K", temperature), 20, 20, 24, WHITE);
        DrawText("UP / DOWN - Change temperature", 20, 60, 20, LIME);
        DrawText("SPACE - Toggle spectrum graph", 20, 90, 20, LIME);
        DrawText("R - Refresh spectrum", 20, 120, 20, LIME);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}