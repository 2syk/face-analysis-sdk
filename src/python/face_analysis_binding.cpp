#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <opencv2/opencv.hpp>

#include "ndarray_converter.h"
#include "tracker/myFaceTracker.hpp"
#include "utils/helpers.hpp"

namespace py = pybind11;

using namespace FACETRACKER;

PYBIND11_MODULE(face_analysis_sdk, m)
{

    NDArrayConverter::init_numpy();

    py::class_<Pose>(m, "Pose")
        .def(py::init<>())
        .def_readwrite("pitch", &Pose::pitch)
        .def_readwrite("yaw", &Pose::yaw)
        .def_readwrite("roll", &Pose::roll);

    py::class_<FaceTrackerParams>(m, "FaceTrackerParams")
        .def(py::init<>());
    // .def("Load", &myFaceTrackerParams::Load)
    // .def("Save", &myFaceTrackerParams::Save);

    py::class_<myFaceTracker>(m, "FaceTracker")
        .def(py::init())
        .def("Load", &myFaceTracker::Load)
        .def("Save", &myFaceTracker::Save)
        .def("NewFrame", &myFaceTracker::NewFrame)
        .def("getPose", &myFaceTracker::getPose)
        .def("getShape", &myFaceTracker::getShape)
        .def("get3DShape", &myFaceTracker::get3DShape)
        .def("Reset", &myFaceTracker::Reset);

    m.def("LoadFaceTrackerParams", (FaceTrackerParams * (*)(const char *)) & LoadFaceTrackerParams);

    m.def("load_grayscale_image", (cv::Mat_<uint8_t>(*)(const char *)) & load_grayscale_image,
          "Loads a grayscale array from an image path",
          py::arg("pathname"));
}
