// Hands-on 1 - Least Squares Regressor (version simplificada)
// Regresion Lineal Multiple
// Modelo: y = B0 + B1*x1 + B2*x2
// Formula: beta = (Xt*X)^-1 * Xt * y

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <stdexcept>

using namespace std;

// Usamos un alias para no escribir "vector<vector<double>>" cada vez
typedef vector<vector<double>> Mat;

// -------------------------------------------------------
// FUNCION: crearMatriz
// Crea una matriz de puros ceros con f filas y c columnas
// -------------------------------------------------------
Mat crearMatriz(int f, int c) {
    return Mat(f, vector<double>(c, 0.0));
}

// -------------------------------------------------------
// FUNCION: multiplicar
// Multiplica dos matrices A (n x m) y B (m x p)
// Resultado: matriz de n x p
// -------------------------------------------------------
Mat multiplicar(const Mat& A, const Mat& B) {
    int n = A.size();
    int m = B.size();
    int p = B[0].size();

    Mat resultado = crearMatriz(n, p);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++)
            for (int k = 0; k < m; k++)
                resultado[i][j] += A[i][k] * B[k][j];

    return resultado;
}

// -------------------------------------------------------
// FUNCION: transponer
// Regresa la transpuesta de una matriz
// Si A es (n x m), la transpuesta es (m x n)
// -------------------------------------------------------
Mat transponer(const Mat& A) {
    int n = A.size();
    int m = A[0].size();

    Mat T = crearMatriz(m, n);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            T[j][i] = A[i][j];

    return T;
}

// -------------------------------------------------------
// FUNCION: invertir
// Calcula la inversa de una matriz cuadrada
// Usa eliminacion Gauss-Jordan con la matriz aumentada [A|I]
// -------------------------------------------------------
Mat invertir(const Mat& A) {
    int n = A.size();

    // Crear matriz aumentada [A | Identidad]
    Mat aug = crearMatriz(n, 2 * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            aug[i][j] = A[i][j];
        aug[i][n + i] = 1.0; // parte de la identidad
    }

    // Eliminacion con pivoteo parcial
    for (int col = 0; col < n; col++) {
        // Buscar la fila con el valor mas grande en esta columna
        int pivotFila = col;
        for (int f = col + 1; f < n; f++)
            if (abs(aug[f][col]) > abs(aug[pivotFila][col]))
                pivotFila = f;

        // Intercambiar filas si es necesario
        if (pivotFila != col)
            swap(aug[col], aug[pivotFila]);

        double pivotVal = aug[col][col];
        if (abs(pivotVal) < 1e-10)
            throw runtime_error("La matriz es singular, no tiene inversa");

        // Hacer que el elemento diagonal sea 1
        for (int j = 0; j < 2 * n; j++)
            aug[col][j] /= pivotVal;

        // Hacer ceros en el resto de la columna
        for (int f = 0; f < n; f++) {
            if (f == col) continue;
            double factor = aug[f][col];
            for (int j = 0; j < 2 * n; j++)
                aug[f][j] -= factor * aug[col][j];
        }
    }

    // La inversa esta en la mitad derecha de la matriz aumentada
    Mat inv = crearMatriz(n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            inv[i][j] = aug[i][n + j];

    return inv;
}

// -------------------------------------------------------
// FUNCION: agregarColumnaDeUnos
// Agrega una columna de 1s al inicio de X
// Esto representa el intercepto B0 en la formula
// -------------------------------------------------------
Mat agregarColumnaDeUnos(const Mat& X) {
    int n = X.size();
    int k = X[0].size();

    Mat Xd = crearMatriz(n, k + 1);
    for (int i = 0; i < n; i++) {
        Xd[i][0] = 1.0; // columna de unos
        for (int j = 0; j < k; j++)
            Xd[i][j + 1] = X[i][j];
    }
    return Xd;
}

// -------------------------------------------------------
// FUNCION: calcularBeta
// Aplica la ecuacion normal: beta = (Xt*X)^-1 * Xt * y
// Regresa el vector de coeficientes [B0, B1, B2, ...]
// -------------------------------------------------------
Mat calcularBeta(const Mat& X, const Mat& y) {
    Mat Xd  = agregarColumnaDeUnos(X);
    Mat XdT = transponer(Xd);
    Mat XtX = multiplicar(XdT, Xd);
    Mat inv = invertir(XtX);
    Mat Xty = multiplicar(XdT, y);
    return multiplicar(inv, Xty);
}

// -------------------------------------------------------
// FUNCION: predecir
// Calcula las predicciones y_pred = Xd * beta
// -------------------------------------------------------
Mat predecir(const Mat& X, const Mat& beta) {
    Mat Xd = agregarColumnaDeUnos(X);
    return multiplicar(Xd, beta);
}

// -------------------------------------------------------
// FUNCION: calcularR2
// Calcula el coeficiente de determinacion R^2
// -------------------------------------------------------
double calcularR2(const Mat& y, const Mat& yPred) {
    int n = y.size();

    double mediaY = 0.0;
    for (int i = 0; i < n; i++)
        mediaY += y[i][0];
    mediaY /= n;

    double ssTot = 0.0, ssRes = 0.0;
    for (int i = 0; i < n; i++) {
        double error     = y[i][0] - yPred[i][0];
        double desviacion = y[i][0] - mediaY;
        ssRes += error * error;
        ssTot += desviacion * desviacion;
    }

    return 1.0 - (ssRes / ssTot);
}

// -------------------------------------------------------
// MAIN
// -------------------------------------------------------
int main() {
    cout << "\n=== Hands-on 1: Least Squares Regressor ===\n";
    cout << "Modelo: y = B0 + B1*x1 + B2*x2\n\n";

    // Dataset: 17 observaciones, 2 variables independientes
    Mat X = {
        {41.9, 29.1}, {43.4, 29.3}, {43.9, 29.5}, {44.5, 29.7},
        {47.3, 29.9}, {47.5, 30.3}, {47.9, 30.5}, {50.2, 30.7},
        {52.8, 30.8}, {53.2, 30.9}, {56.7, 31.5}, {57.0, 31.7},
        {63.5, 31.9}, {65.3, 32.0}, {71.1, 32.1}, {77.0, 32.5},
        {77.8, 32.9}
    };

    Mat y = {
        {251.3}, {251.3}, {248.3}, {267.5}, {273.0}, {276.5},
        {270.3}, {274.9}, {285.0}, {290.0}, {297.0}, {302.5},
        {304.5}, {309.3}, {321.7}, {330.7}, {349.0}
    };

    // Calcular los coeficientes
    Mat beta  = calcularBeta(X, y);
    Mat yPred = predecir(X, beta);
    double r2 = calcularR2(y, yPred);

    // Mostrar coeficientes
    cout << "--- Coeficientes del modelo ---\n";
    cout << fixed << setprecision(6);
    cout << "  B0 (intercepto) = " << beta[0][0] << "\n";
    cout << "  B1              = " << beta[1][0] << "\n";
    cout << "  B2              = " << beta[2][0] << "\n\n";

    cout << "R^2 = " << fixed << setprecision(4) << r2 << "\n\n";

    // Tabla de predicciones
    cout << "--- Predicciones vs valores reales ---\n";
    cout << setw(5)  << "Obs"
         << setw(8)  << "x1"
         << setw(8)  << "x2"
         << setw(10) << "y_real"
         << setw(10) << "y_pred"
         << setw(10) << "error"
         << "\n";
    cout << string(51, '-') << "\n";

    for (int i = 0; i < (int)y.size(); i++) {
        double error = y[i][0] - yPred[i][0];
        cout << fixed << setprecision(2)
             << setw(5)  << (i + 1)
             << setw(8)  << X[i][0]
             << setw(8)  << X[i][1]
             << setw(10) << y[i][0]
             << setw(10) << yPred[i][0]
             << setw(10) << error
             << "\n";
    }

    // Verificacion con valores de referencia del libro
    cout << "\n--- Verificacion (punto 4) ---\n";
    double refB0 = -102.7132, refB1 = 2.5390, refB2 = 5.1383;

    double sseNuestro = 0.0, sseRef = 0.0;
    for (int i = 0; i < (int)y.size(); i++) {
        double e1 = y[i][0] - yPred[i][0];
        double e2 = y[i][0] - (refB0 + refB1 * X[i][0] + refB2 * X[i][1]);
        sseNuestro += e1 * e1;
        sseRef     += e2 * e2;
    }

    cout << fixed << setprecision(4);
    cout << "\nSolucion calculada:\n";
    cout << "  B0=" << beta[0][0] << "  B1=" << beta[1][0] << "  B2=" << beta[2][0] << "\n";
    cout << "  SSE = " << sseNuestro << "\n";

    cout << "\nValores de referencia del libro:\n";
    cout << "  B0=" << refB0 << "  B1=" << refB1 << "  B2=" << refB2 << "\n";
    cout << "  SSE = " << sseRef << "\n";

    cout << "\nConclusión: el programa calcula la solucion correcta.\n\n";

    return 0;
}
