class YRTRClient {
  #onStateUpdate;
  #socket;
  #getStateCount;
  #maxGetState;
  #connectionTimeout; // ms
  #reconnectInterval; // ms
  #getStateInterval; // ms
  #stopRequested;

  constructor(uri, onStateUpdate) {
    this.uri = uri;
    this.#onStateUpdate = onStateUpdate;
    this.#socket = null;
    this.#getStateCount = 0;
    this.#maxGetState = 1;
    this.#connectionTimeout = 50; // ms
    this.#reconnectInterval = 1000; // ms
    this.#getStateInterval = 300; // ms
    this.#stopRequested = false;
  }

  connect() {
    if (this.#stopRequested) return;

    this.#socket = new WebSocket(this.uri);

    this.#socket.onopen = () => {
      console.log('WebSocket connection established');
      setInterval(() => {
        if (!this.#stopRequested) {
          this.#getState();
        }
      }, this.#getStateInterval);
    };

    this.#socket.onmessage = (event) => {
      this.#handleMessage(event.data);
    };

    this.#socket.onclose = (event) => {
      console.log('WebSocket connection closed', event);
      if (!this.#stopRequested) {
        setTimeout(() => this.connect(), this.#reconnectInterval);
      }
    };

    this.#socket.onerror = (error) => {
      console.error('WebSocket error:', error);
    };
  }

  disconnect() {
    this.#stopRequested = true;
    if (this.#socket) {
      this.#socket.close();
    }
  }

  sendInput(label, value) {
    const event = {
      type: 'input',
      label: label,
      val: value
    };

    this.#sendData(event);
  }

  sendButton(label) {
    const event = {
      type: 'button',
      label: label,
      val: -1
    };

    this.#sendData(event);
  }

  sendCheckbox(label, activate) {
    const event = {
      type: 'checkbox',
      label: label,
      val: activate
    };

    this.#sendData(event);
  }

  sendProtectedList(sideMap) {
    const event = {
      type: 'protected_list',
      label: FnLabel.kInvalid,
      val: sideMap
    };

    this.#sendData(event);
  }

  #handleMessage(data) {
    try {
      const message = JSON.parse(data);

      switch (message.type) {
        case 'get_state':
          this.#handleGetState(message);
          break;
        // Add other message types if needed
        default:
          console.warn('Unknown message type:', message.type);
      }
    } catch (error) {
      console.error('Error processing message:', error);
    }
  }

  #handleGetState(message) {
    if (message.type !== 'get_state' ||
      message.label !== FnLabel.kInvalid ||
      !message.val) {
      console.error('Invalid get_state message format');
      return;
    }

    this.#getStateCount--;
    this.#onStateUpdate(message.val);
  }

  #getState() {
    if (this.#getStateCount >= this.#maxGetState) {
      console.log('Too many get_state requests pending');
      return;
    }

    const event = {
      type: 'get_state',
      label: FnLabel.kInvalid,
      val: {}
    };

    this.#sendData(event);
    this.#getStateCount++;
  }

  #sendData(data) {
    if (!this.#socket || this.#socket.readyState !== WebSocket.OPEN) {
      console.error('WebSocket is not connected, drop data');
      return;
    }

    try {
      this.#socket.send(JSON.stringify(data));
    } catch (error) {
      console.error('Error sending data:', error);
    }
  }
}
