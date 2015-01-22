



std::queue<shared_ptr<message>> messageInQueue;
boost::condition                messageAvailable;
boost::mutex                    messageInQueue_Mutex;


// Message Recv Thread

void recvMessage()
{
    Message buffer;
    
    bool shouldBlock = true;
    
    while (true)
    {
        if (recv(buffer,shouldBlock))
        {
            boost::mutex::scoped_lock lock(messageInQueue_Mutex);
            messageInQueue.push(new Message(buffer));
            messageAvailable.notify_one();
        }
        
        
    }
    
}

// Message Process Thread

void processMessage()
{
    shared_ptr<Message> buffer = 0;
    
    while(true)
    {
        {
            boost::mutex::scoped_lock lock(messageInQueue_Mutex);
            // on entry, releases mutex and suspends this thread
            // on return, reacquires mutex
            while(messageInQueue.empty())
            {
                messageAvailable.wait(lock);
            }
            
            buffer = messageInQueue.pop();
        }
        
        // do something with buffer
        if (buffer)
        {
            boost::thread processBuffer_Thread(processBuffer,buffer);
            buffer.reset();
        }
    }
    
    
}

void processBuffer(shared_ptr<Message> buffer)
{
    // do something with message
}







// Message Sending

// Sample Functions that Creates Message to Send
void someFunction
{
    shared_ptr<Message> buffer = new Message();
    
    // TODO: populate the message with data
    
    // might not want to block this function by waiting for the Out Queue to become available
    boost::thread addToOutMessageQueue_Thread(addToOutMessageQueue,buffer);
    
}

void addToOutMessageQueue(shared_ptr<Message> buffer)
{
    boost::mutex::scoped_lock lock(messageOutQueue_Mutex);
    messageOutQueue.push(buffer);
    messageOutAvailable.notify_one();
}

void sendMessage()
{
    shared_ptr<Message> buffer = 0;
    
    while(true)
    {
        {
            boost::mutex::scoped_lock lock(messageOutQueue_Mutex);
            // on entry, releases mutex and suspends this thread
            // on return, reacquires mutex
            while(messageOutQueue.empty())
            {
                messageOutAvailable.wait(lock);
            }
            
            buffer = messageOutQueue.pop();
        }
        
        // send the Message
        if (buffer)
        {
            boost::thread sendBuffer_Thread(sendBuffer,buffer);
            buffer.reset();
        }
    
    
}

void sendBuffer()
