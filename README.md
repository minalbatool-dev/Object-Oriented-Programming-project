# Social Network App (Console-Based)

A console-based social network simulation built in **C++** for my
**Object-Oriented Programming** course (Semester 2).

> **Note:** This project was completed during my 2nd semester and is being
> uploaded now for portfolio/reference purposes.

## Overview

The application simulates core social network behavior: users can have friends,
like pages, create posts (with activities like *feeling*, *celebrating*,
*thinking about*), comment on posts, like posts, and share memories of old posts.
All entities (Users, Pages, Posts, Comments, Activities, Memories) are modeled
as objects with unique IDs.

## Features

- **Set current user** — everything is viewed from that user's perspective
- **View Home** — posts from friends and liked pages from the last 24 hours
- **Like a post** (max 10 likes per post)
- **View list of people who liked a post**
- **Comment on a post** (max 10 comments per post)
- **View a post** with its comments
- **Share a memory** — reshare an old post with new text
- **View user profile / timeline**
- **View friend list** and **liked pages**
- **View a page** and its timeline
- **View "On This Day" memories**

## OOP Concepts Demonstrated

- Classes & Objects (User, Page, Post, Comment, Activity, Memory)
- **Inheritance** (Memory inherits from Post; Activity-typed posts)
- **Polymorphism** (post rendering differs by type — simple post, activity post, memory)
- **Encapsulation** (private data with controlled access)
- **Composition / Aggregation** (User → Posts, Page → Posts, Post → Comments)
- **Separation of concerns** — driver logic lives in `SocialNetworkApp`, not `main()`



## Project Structure
