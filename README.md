# CIT Campus Navigator - Interactive Map

An interactive campus navigation system for Coimbatore Institute of Technology with Google Maps-style features.

## Features

### 🗺️ Interactive Campus Map
- Detailed SVG-based campus map with all major buildings and facilities
- Accurate road network representation
- Color-coded areas (Green spaces, Academic buildings, Hostels/Admin)
- Clickable location pins for easy navigation

### 🛣️ Shortest Path Finding
- **Floyd-Warshall Algorithm** (O(V³)) for all-pairs shortest paths
- 60+ road junction nodes for accurate routing
- Real-time path calculation between any two locations
- Distance display in map units

### ✨ Google Maps-Style Animations
- **Animated route drawing** with smooth path reveal
- **Moving marker** that travels along the route
- Pulsing glow effect on the animated marker
- Direction arrow showing travel direction
- Looping animation for continuous visualization

### 🎮 Interactive Controls
- **Pan & Zoom**: Mouse drag to pan, scroll to zoom
- **Touch support**: Pinch to zoom, drag to pan on mobile
- **Zoom controls**: +/- buttons and reset view
- **Animation controls**: Pause/resume the moving marker
- **Pin selection**: Click pins on map or use dropdowns

### 📍 26 Campus Locations
Including:
- Main Gate, Auditorium, Main Block
- Academic buildings (Mechanical LAB, IT Block, Library, MSC Block)
- Sports facilities (Football Ground, Cricket Ground, Basketball Court, Badminton Court)
- Hostels (Palani, Thirumalai, Velingiri, Kailash, Maruthamalai, Women's)
- Amenities (Canteen, A Mess, B Mess, Dispensary, CIT Store)
- And more!

## How to Use

1. **Open `map.html`** in a modern web browser
2. **Select locations**:
   - Use the "From" and "To" dropdowns, OR
   - Click location pins directly on the map
3. **Find Path**: Click the "Find Path" button
4. **Watch the animation**: 
   - Blue route draws automatically
   - Animated marker travels along the path
   - Use "Pause/Resume" to control animation
5. **Navigate the map**:
   - Drag to pan
   - Scroll or use +/- buttons to zoom
   - Click reset (⌂) to return to default view

## Technical Details

### Algorithm
- **Floyd-Warshall** shortest path algorithm
- Preprocesses all-pairs shortest paths in O(V³) time
- Instant path queries after preprocessing
- Road-accurate routing (follows actual campus roads)

### Architecture
- Pure HTML/CSS/JavaScript (no external dependencies)
- SVG-based rendering for crisp graphics at any zoom level
- Efficient animation using `requestAnimationFrame`
- Responsive design works on desktop and mobile

### Files
- `map.html` - Main interactive map application
- `shortest.c` - C implementation of Floyd-Warshall (reference)
- `collegemap.png` - Campus map reference image
- `azure-maps-animations.js` - Animation library (optional)
- `Animate a GPS trace.html` - GPS animation example
- `Animate along a route path.html` - Route animation example

## Browser Compatibility
- Chrome/Edge (recommended)
- Firefox
- Safari
- Mobile browsers (iOS Safari, Chrome Mobile)

## Future Enhancements
- Real-time location tracking
- Multiple route options
- Walking time estimates
- Building information popups
- Search functionality
- 3D building views
- Indoor navigation

## Credits
Developed for Coimbatore Institute of Technology
Algorithm: Floyd-Warshall shortest path
Design inspired by Google Maps

---

**Note**: This is a demonstration project. Map coordinates and distances are in arbitrary map units, not real-world meters.
