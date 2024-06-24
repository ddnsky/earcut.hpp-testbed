#pragma once
// FIXME: clangd #include <mapbox/earcut.hpp>
#include "../../include/mapbox/earcut.hpp"

#include <array>
#include <memory>
#include <vector>

template <typename Coord, typename Polygon>
class EarcutTesselator {
public:
    using Vertex = std::array<Coord, 2>;
    using Vertices = std::vector<Vertex>;
    using Indices = std::vector<uint32_t>;
    struct Result {
        Vertices const& vertices;
        Indices const& indices;
    };


    EarcutTesselator(const Polygon &polygon_)
        : polygon(polygon_)
    {
        for (const auto& ring : polygon_) {
            for (const auto& vertex : ring) {
                vertices_.emplace_back(Vertex {{ Coord(std::get<0>(vertex)),
                                                 Coord(std::get<1>(vertex)) }});
            }
        }
    }

    EarcutTesselator & operator=(const EarcutTesselator&) = delete;

    Result run() {
        indices_ = mapbox::earcut(polygon);
        return {vertices_, indices_};
    }

    std::vector<uint32_t> const& indices() const {
        return indices_;
    }

    Vertices const& vertices() const {
        return vertices_;
    }

    const char *name() { 
        return "earcut"; 
    }

private:
    const Polygon &polygon;
    Vertices vertices_;
    std::vector<uint32_t> indices_;
};
