/* Copyright 2024 The JAX Authors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "mlir-c/IR.h"
#include "mlir/Bindings/Python/PybindAdaptors.h"
#include "pybind11/detail/common.h"
#include "jaxlib/triton/triton_dialect_capi.h"

namespace py = pybind11;

PYBIND11_MODULE(_triton_ext, m) {
  //
  // Dialects.
  //

  m.def(
      "register_dialect",
      [](MlirContext context, bool load) {
        MlirDialectHandle dialect = mlirGetDialectHandle__triton__();
        mlirDialectHandleRegisterDialect(dialect, context);
        if (load) {
          mlirDialectHandleLoadDialect(dialect, context);
        }
      },
      py::arg("context"), py::arg("load") = true);

  //
  // Types.
  //

  mlir::python::adaptors::mlir_type_subclass(m, "PointerType",
                                             mlirTritonIsAPointerType)
      .def_classmethod(
          "get",
          [](py::object cls, MlirType pointee_type, int64_t address_space) {
            return cls(mlirTritonPointerTypeGet(pointee_type, address_space));
          },
          py::arg("cls"), py::arg("pointee_type"), py::arg("address_space"),
          "Creates a PointerType type.")
      .def_property_readonly("pointee_type", [](MlirType self) {
        return mlirTritonPointerTypeGetPointeeType(self);
      });
}
