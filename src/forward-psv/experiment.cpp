//
// Created by lars on 17.03.18.
//

#include <omp.h>
#include "experiment.h"
#include "propagator.h"

experiment::experiment(arma::imat _receivers, arma::imat _sources, arma::vec _sourceFunction, int nt, double dt) {
    receivers = std::move(_receivers);
    sources = std::move(_sources);
    sourceFunction = std::move(_sourceFunction);
    timestep = dt;
    experimentSteps = nt;
    experimentTime = (nt - 1) * dt;

    // This way all shots have the same receivers and sources
    for (int iShot = 0; iShot < sources.n_rows; ++iShot) {
        shots.emplace_back(shot(sources.row(iShot), receivers, sourceFunction, experimentSteps));
    }
}

void experiment::forwardData() {

    std::cout << "Starting forward modeling of shots." << std::endl;
    std::cout << "Total of " << sources.n_rows << " shots." << std::endl;

    double startTime = omp_get_wtime();

    // Run forward simulation for all shots
    for (int iShot = 0; iShot < sources.n_rows; ++iShot) {
        // This directly modifies the forwardData fields
        std::cout << " -- Running shot: " << iShot + 1 << std::endl;
        propagator::propagate(currentModel, false, false, shots[iShot].receivers, shots[iShot].source,
                              shots[iShot].sourceFunction, shots[iShot].forwardData_vx, shots[iShot].forwardData_vz,
                              experimentSteps, timestep, true);
        std::cout << "    Done! " << std::endl;
    }

    double stopTime = omp_get_wtime();
    double secsElapsed = stopTime - startTime; // that's all !
    std::cout << "Finished forward modeling of shots, elapsed time: " << secsElapsed << " seconds (wall)." << std::endl
              << std::endl;
}
