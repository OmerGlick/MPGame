# app.py
from fastapi import FastAPI, Depends, HTTPException
from auth import create_user, authenticate_user, get_db
from models import init_db, User
from sqlalchemy.orm import Session
from room import create_room, join_room, leave_room
from pydantic import BaseModel

class RoomCreateRequest(BaseModel):
    room_name: str
    player_id: int

class RoomJoinRequest(BaseModel):
    room_name: str
    player_id: int

class RoomLeaveRequest(BaseModel):
    room_name: str
    player_id: int

class UserRequest(BaseModel):
    username: str
    password: str

app = FastAPI()

# Initialize database
init_db()

@app.post("/register")
def register(request: UserRequest, db: Session = Depends(get_db)):
    existing_user = db.query(User).filter(User.username == request.username).first()
    if existing_user:
        raise HTTPException(status_code=400, detail="Username already taken")
    user = create_user(db, request.username, request.password)
    return {"msg": "User registered successfully", "user_id": user.id}

@app.post("/login")
def login(request: UserRequest, db: Session = Depends(get_db)):
    user = authenticate_user(db, request.username, request.password)
    return {"msg": "Login successful", "user_id": user.id}

@app.post("/create-room")
def create_room_endpoint(request: RoomCreateRequest, db: Session = Depends(get_db)):
    room = create_room(db, request.room_name, request.player_id)
    return {"msg": "Room created successfully", "room_id": room.id}

@app.post("/join-room")
def join_room_endpoint(request: RoomJoinRequest, db: Session = Depends(get_db)):
    room = join_room(db, request.room_name, request.player_id)
    return {"msg": "Joined room successfully", "room_id": room.id}

@app.post("/leave-room")
def leave_room_endpoint(request: RoomLeaveRequest, db: Session = Depends(get_db)):
    leave_room(db, request.room_name, request.player_id)
    return {"msg": "Left room successfully"}
