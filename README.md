# Advanced Procedural Spherical Map Generation via OpenGL

Introducing a cutting-edge approach to rendering: a procedurally crafted, high-resolution 3D spheroid surface infused with dynamic noise modulation techniques.

---

## Visual Demonstration: Precision Surface Construction

<p align="center">
  <img src="patchDemo.gif" width="700" height="500" alt="Detailed demonstration of the procedural surface construction"/>
</p>

---

## Comparative Visualization: Surface Rendering alongside its Reference Sphere

<p align="center">
  <img src="video.gif" width="700" height="500" alt="Comparison between the rendered surface and its reference sphere"/>
</p>

---

## In-depth Project Synopsis:

Harnessing the capabilities of OpenGL, we present an intricately engineered application tailored for the procedural generation of a 3D spheroid surface. Beyond mere construction, the application delves into the realm of enriched noise modulation for heightened realism.

### Core Surface Architecture:

- The foundational bedrock of the 3D spheroid's surface is sculpted using a plethora of triangular facets.
- Pursuing granularity, each primary triangular facet undergoes a rigorous subdivision regimen, birthing a series of refined sub-triangular elements.
- The magic of Perlin noise is meticulously applied to each of these subdivided triangles. This instills them with a uniquely pseudorandom texture, paving the way for the birth of detailed terrains and topographical nuances.

### Superior Memory Management & Storage Techniques:

- With a nod to efficiency and rapid data accessibility, each triangular facet's spatial coordinates undergo a hashing process. This culminates in a distinct key, ensconced within a purpose-built hash table.
- Memory optimization takes center stage: our system judiciously retains only the triangular facets within the camera's purview. Marginal facets, distant from the observer's focal field, are programmatically expunged.
- In scenarios of expansive celestial bodies, this selective retention eliminates redundancies. By circumventing the storage of inconspicuous surface data, RAM conservation is achieved.
- Topping off our commitment to optimal performance, facets that are evicted from memory don't just vanish—they're reprocessed and reincarnated, ready for future rendering tasks.

### Application Hallmarks:

At its essence, our toolset offers a limitless horizon, empowering users to generate vast, procedurally conceived celestial surfaces. Regardless of the planetary expanse under consideration, memory efficiency remains uncompromised, guaranteeing uninterrupted and stellar performance.

