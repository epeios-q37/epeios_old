/** Votre premier programme Java */
import java.io.*;
import org.zeusw.*;
import javax.xml.parsers.*;
import org.w3c.dom.*;
import org.xml.sax.*;


class XMLPreprocessorDemo {

	static {
		System.loadLibrary("jexpp");
	}
	public static void main (
		String[] args )	throws Exception
		{
			   DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			   DocumentBuilder builder = factory.newDocumentBuilder();
			   File file = new File("../Contacts.xml");
				FileInputStream objFin = new FileInputStream(file);
				XPPInputStream obj = new XPPInputStream(objFin);
			   
			   Document doc = builder.parse(obj);
			   NodeList list = doc.getElementsByTagName("*");
			   System.out.println("XML Elements: ");
			   for (int i=0; i<list.getLength(); i++) {
				   Element element = (Element)list.item(i);
				   System.out.print(element.getNodeName());
				   if ( ( element.getFirstChild() != null ) && ( element.getFirstChild().getNodeValue().trim().length() != 0 ) ) {
					   System.out.print( " : " );
					System.out.println(element.getFirstChild().getNodeValue());
					} else
					System.out.println();
			   }
	}
}

