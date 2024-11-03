# Deribit Trading Execution and Management System

This project is a C++ implementation of an order execution and management system for trading on the [Deribit Test Platform](https://test.deribit.com/). The system allows users to place and cancel orders, retrieve order book data, and view their current positions. 

## Features

- **Authentication**: Securely handles authentication using client ID and client secret to retrieve an access token.
- **Order Placement**: Supports placing orders with multiple order types (limit, market, stop, etc.).
- **Order Management**: Includes functions to cancel existing orders.
- **Order Book Retrieval**: Retrieves live order book data for selected instruments.
- **Position Viewing**: Displays the user’s current open positions on Deribit.

## Libraries Used

- **libcurl**: Used for making HTTP requests to the Deribit API.
- **nlohmann JSON**: For handling JSON data returned by the API.

## Project Structure

The project is organized into the following files:

- `authenticateClass.h/.cpp`: Manages authentication and API communication. It handles the generation of access tokens, communication with Deribit’s API endpoints, and stores the credentials securely.
- `infoGather.h/.cpp`: Collects and provides secure access to API keys and client IDs.
- `inputValidator.h`: Provides inline functions to validate user input, ensuring smooth interaction even with invalid inputs.

## Setup and Installation

### Prerequisites

- C++ compiler (e.g., G++, Clang)
- [libcurl](https://curl.se/libcurl/) library
- [nlohmann JSON](https://github.com/nlohmann/json) library

