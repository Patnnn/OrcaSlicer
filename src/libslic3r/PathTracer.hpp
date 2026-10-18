#pragma once

#include "Point.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <vector>

namespace Slic3r {

struct PathTraceTriangle
{
    Vec3f a{Vec3f::Zero()};
    Vec3f b{Vec3f::Zero()};
    Vec3f c{Vec3f::Zero()};
    Vec3f albedo{Vec3f::Constant(0.72f)};
};

struct alignas(16) PathTraceGpuTriangle
{
    std::array<float, 4> a{};
    std::array<float, 4> b{};
    std::array<float, 4> c{};
    std::array<float, 4> albedo{};
};

struct alignas(16) PathTraceGpuNode
{
    std::array<float, 4> bounds_min{};
    std::array<float, 4> bounds_max{};
    std::array<uint32_t, 4> data{};
};

struct PathTraceGpuScene
{
    std::vector<PathTraceGpuTriangle> triangles;
    std::vector<PathTraceGpuNode> nodes;
};

void append_path_trace_triangles(std::vector<PathTraceTriangle>& triangles,
                                 const std::vector<Vec3f>& vertices,
                                 const std::vector<Vec3i32>& indices,
                                 const Transform3d& transform,
                                 const Vec3f& albedo);

PathTraceGpuScene build_path_trace_gpu_scene(const std::vector<PathTraceTriangle>& triangles);

struct PathTraceCamera
{
    Vec3f position{Vec3f::Zero()};
    Vec3f right{Vec3f::UnitX()};
    Vec3f up{Vec3f::UnitY()};
    Vec3f forward{-Vec3f::UnitZ()};
    bool orthographic{false};
    float orthographic_height{0.0f};
    float vertical_fov_degrees{45.0f};
};

struct PathTraceSettings
{
    uint32_t width{960};
    uint32_t height{720};
    uint32_t samples_per_pixel{16};
    uint32_t sample_batch_size{4};
    uint32_t max_bounces{3};
};

struct PathTraceImage
{
    uint32_t width{0};
    uint32_t height{0};
    uint32_t samples_per_pixel{0};
    std::vector<uint8_t> rgb;
    bool canceled{false};
};

PathTraceImage render_path_traced_image(const std::vector<PathTraceTriangle>& triangles,
                                        const PathTraceCamera& camera,
                                        const PathTraceSettings& settings,
                                        const std::function<bool()>& was_canceled = {},
                                        const std::function<void(float)>& report_progress = {},
                                        const std::function<void(const PathTraceImage&)>& report_image = {});

} // namespace Slic3r