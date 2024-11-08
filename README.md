# Real-Time Multiplayer Game Backend

## Overview
This project is a backend server for a real-time multiplayer game, designed to support multiple players, game room creation, and real-time interactions. The backend uses a combination of **Python** (for REST API endpoints) and **C++** (for WebSocket server and game state management), aiming to provide a seamless gaming experience with minimal latency.

## Features
- **User Authentication**: REST API endpoints for user registration and login, ensuring secure access to the game.
- **Game Rooms**: Allows players to create and join game rooms, each with an isolated game state.
- **Real-Time Communication**: C++ WebSocket server handles real-time updates between the server and clients.
- **Multithreading**: Efficient game state management with concurrency for smooth, simultaneous interactions.
  
## Tech Stack
- **Python**: FastAPI for REST API and user management.
- **C++**: WebSocket server and game loop, leveraging multithreading for real-time communication.
- **SQLite** (or PostgreSQL): For user and room data storage.
- **Docker** (optional): For containerized deployment and consistency across environments.

## Setup
### Prerequisites
- Python 3.8+
- C++ compiler supporting C++11 or later
- SQLite or PostgreSQL
- Visual Studio Code (recommended for WSL integration on Windows)
- WSL (optional, for Windows users)

### Installation
1. **Clone the Repository:**
   ```bash
   git clone https://github.com/yourusername/repo-name.git
   cd repo-name
   ```

2. **Set Up the Python Environment**:
   - Create and activate a virtual environment:
     ```bash
     python3 -m venv venv
     source venv/bin/activate  # On Windows: venv\Scripts\activate
     ```
   - Install dependencies:
     ```bash
     pip install -r requirements.txt
     ```

3. **Compile the C++ Server (To Be Added)**
   - Follow the instructions in the `backend/` folder for compiling the C++ WebSocket server.

## Usage
1. **Run the REST API**:
   ```bash
   uvicorn api.app:app --reload
   ```
2. **Run the C++ WebSocket Server** (instructions in `backend/README.md`).

## Contributing
Contributions are welcome! Please open an issue or submit a pull request with any improvements.
