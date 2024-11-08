# app.py
from fastapi import FastAPI, Depends, HTTPException
from auth import create_user, authenticate_user, get_db
from models import init_db, User
from sqlalchemy.orm import Session

app = FastAPI()

# Initialize database
init_db()

@app.post("/register")
def register(username: str, password: str, db: Session = Depends(get_db)):
    existing_user = db.query(User).filter(User.username == username).first()
    if existing_user:
        raise HTTPException(status_code=400, detail="Username already taken")
    user = create_user(db, username, password)
    return {"msg": "User registered successfully", "user_id": user.id}

@app.post("/login")
def login(username: str, password: str, db: Session = Depends(get_db)):
    user = authenticate_user(db, username, password)
    return {"msg": "Login successful", "user_id": user.id}
