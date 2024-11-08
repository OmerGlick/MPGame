# test_models.py
from models import init_db, SessionLocal, User, Room

def test_models():
    # Initialize the database
    init_db()

    # Create a new session
    db = SessionLocal()

    try:
        # **Create a New User**
        new_user = User(username="player1", password="hashedpassword")
        db.add(new_user)
        db.commit()
        db.refresh(new_user)
        print(f"User created: {new_user.username}, ID: {new_user.id}")

        # **Query the User**
        user = db.query(User).filter_by(username="player1").first()
        print(f"Queried User: {user.username}, ID: {user.id}")

        # **Create a New Room**
        new_room = Room(room_name="Room1", player1_id=new_user.id)
        db.add(new_room)
        db.commit()
        db.refresh(new_room)
        print(f"Room created: {new_room.room_name}, ID: {new_room.id}")

        # **Access the Relationship**
        room = db.query(Room).filter_by(room_name="Room1").first()
        print(f"Room '{room.room_name}' has player1: {room.player1.username}")

        # **Update User Information**
        user.username = "player1_updated"
        db.commit()
        db.refresh(user)
        print(f"Updated User: {user.username}")

        # **Delete the Room**
        db.delete(room)
        db.commit()
        print(f"Room '{room.room_name}' deleted.")

    except Exception as e:
        print(f"An error occurred: {e}")
        db.rollback()
    finally:
        db.close()

if __name__ == "__main__":
    test_models()
