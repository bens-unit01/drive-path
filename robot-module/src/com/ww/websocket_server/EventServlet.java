package com.ww.websocket_server;
/*
 * source : https://github.com/jetty-project/embedded-jetty-websocket-examples
 */
import org.eclipse.jetty.websocket.servlet.WebSocketServlet;
import org.eclipse.jetty.websocket.servlet.WebSocketServletFactory;



@SuppressWarnings("serial")
public class EventServlet extends WebSocketServlet
{
    @Override
    public void configure(WebSocketServletFactory factory)
    {
        factory.register(EventSocket.class);
    }
}
