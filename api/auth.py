# auth.py

from models import User, SessionLocal
from sqlalchemy.orm import Session
from passlib.hash import bcrypt
from fastapi import HTTPException, Depends
from fastapi.security import OAuth2PasswordBearer

# OAuth2PasswordBearer defines the token URL for user authentication
oauth2_scheme = OAuth2PasswordBearer(tokenUrl="token")

def get_db() -> Session:
    """
    Provides a database session for dependency injection in FastAPI endpoints.

    Yields:
        Session: A SQLAlchemy session connected to the database.
    """
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

def create_user(db: Session, username: str, password: str) -> User:
    """
    Creates a new user with a hashed password and stores it in the database.

    Args:
        db (Session): The SQLAlchemy session.
        username (str): The desired username of the new user.
        password (str): The plain-text password of the new user.

    Returns:
        User: The created User object with a hashed password.
    """
    # Hash the password using bcrypt
    hashed_password = bcrypt.hash(password)
    # Create a new User object
    new_user = User(username=username, password=hashed_password)
    # Add and commit the new user to the database
    db.add(new_user)
    db.commit()
    db.refresh(new_user)  # Refresh to get the new user's ID
    return new_user

def authenticate_user(db: Session, username: str, password: str) -> User:
    """
    Authenticates a user by verifying the provided username and password.

    Args:
        db (Session): The SQLAlchemy session.
        username (str): The username to look up.
        password (str): The plain-text password to verify.

    Raises:
        HTTPException: If the user is not found or the password is incorrect.

    Returns:
        User: The authenticated User object if credentials are valid.
    """
    # Query the database for a user with the specified username
    user = db.query(User).filter(User.username == username).first()
    # Verify that the user exists and that the password matches
    if user and bcrypt.verify(password, user.password):
        return user
    # Raise an exception if authentication fails
    raise HTTPException(status_code=400, detail="Invalid credentials")
