from sqlalchemy.orm import Session
from models import Room, User
from fastapi import HTTPException

def create_room(db: Session, room_name: str, player_id: int) -> Room:
    """
    Creates a new room with the given name and assigns the first player.
    """
    # Check if the room name is already taken
    existing_room = db.query(Room).filter_by(room_name=room_name).first()
    if existing_room:
        raise HTTPException(status_code=400, detail="Room name already taken")
    
    # Create a new room and assign player1
    new_room = Room(room_name=room_name, player1_id=player_id)
    db.add(new_room)
    db.commit()
    db.refresh(new_room)
    return new_room

def join_room(db: Session, room_name: str, player_id: int) -> Room:
    """
    Allows a user to join an existing room as player2.
    """
    room = db.query(Room).filter_by(room_name=room_name).first()
    if not room:
        raise HTTPException(status_code=404, detail="Room not found")
    
    # Check if the room is full
    if room.player1_id and room.player2_id:
        raise HTTPException(status_code=400, detail="Room is full")

    # Join as player2 if player1 is already assigned
    if room.player1_id != player_id and not room.player2_id:
        room.player2_id = player_id
        db.commit()
        db.refresh(room)
    else:
        raise HTTPException(status_code=400, detail="User is already in the room or room is full")
    
    return room

def leave_room(db: Session, room_name: str, player_id: int):
    """
    Allows a user to leave a room. Removes them from player1 or player2 spot.
    """
    room = db.query(Room).filter_by(room_name=room_name).first()
    if not room:
        raise HTTPException(status_code=404, detail="Room not found")

    # Remove player from the room
    if room.player1_id == player_id:
        room.player1_id = room.player2_id
        room.player2_id = None
    elif room.player2_id == player_id:
        room.player2_id = None
    else:
        raise HTTPException(status_code=400, detail="User is not in the room")

    # Delete room if empty
    if room.player1_id is None and room.player2_id is None:
        db.delete(room)
    db.commit()
